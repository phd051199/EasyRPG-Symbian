#!/usr/bin/env python3
"""Generate a lightweight Symbian build copy of liblcf reader_struct.cpp.

The vendored upstream source remains byte-for-byte unchanged. The generated
copy only replaces two iostream diagnostics in writer-side code with stdio so
that the read path does not pull libc++ iostream/locale into the E72 binary.
"""

from __future__ import annotations

from pathlib import Path
import sys


PROJECT_ROOT = Path(__file__).resolve().parents[1]
SOURCE = PROJECT_ROOT / "third_party" / "liblcf-0.5.0" / "src" / "reader_struct.cpp"

INCLUDE_BLOCK = "#include <iostream>\n#include <iomanip>\n"
INCLUDE_REPLACEMENT = "#include <stdio.h>\n"

FIELD_ORDER_LOG = """\t\tif (field->id < last)\n\t\t\tstd::cerr << \"field order mismatch: \" << field->id\n\t\t\t\t\t  << \" after \" << last\n\t\t\t\t\t  << \" in struct \" << name\n\t\t\t\t\t  << std::endl;\n"""
FIELD_ORDER_LOG_REPLACEMENT = """\t\tif (field->id < last)\n\t\t\tfprintf(stderr, \"field order mismatch: %d after %d in struct %s\\n\", field->id, last, name);\n"""


def replace_exact(text: str, old: str, new: str, expected_count: int, label: str) -> str:
    count = text.count(old)
    if count != expected_count:
        raise RuntimeError(f"expected exactly {expected_count} {label}, found {count}")
    return text.replace(old, new)


def main() -> int:
    if len(sys.argv) != 2:
        print(f"usage: {Path(sys.argv[0]).name} OUTPUT.cpp", file=sys.stderr)
        return 2

    destination = Path(sys.argv[1])
    try:
        text = SOURCE.read_text(encoding="utf-8")
        text = replace_exact(text, INCLUDE_BLOCK, INCLUDE_REPLACEMENT, 1, "iostream include block")
        text = replace_exact(text, FIELD_ORDER_LOG, FIELD_ORDER_LOG_REPLACEMENT, 2, "field-order diagnostics")
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text, encoding="utf-8")
    except (OSError, RuntimeError) as exc:
        print(f"generate_symbian_reader_struct.py: {exc}", file=sys.stderr)
        return 1

    print(f"Generated {destination}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
