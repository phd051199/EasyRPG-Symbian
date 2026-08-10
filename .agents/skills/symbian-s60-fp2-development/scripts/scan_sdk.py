#!/usr/bin/env python3
"""Index an installed S60 3rd FP2 SDK and optionally search symbols."""
from __future__ import annotations
import argparse
from pathlib import Path
import json
import os
import re

CLASS_RE = re.compile(r"\b(?:class|struct)\s+(?:IMPORT_C\s+|EXPORT_C\s+)?([A-Z]\w+)")
METHOD_RE = re.compile(r"\b(?:IMPORT_C|EXPORT_C)\s+[^;{}]+?\b([A-Za-z_]\w*)\s*\(", re.S)
LIB_RE = re.compile(r"^\s*LIBRARY\s+(.+?)\s*$", re.I | re.M)
CAP_RE = re.compile(r"^\s*CAPABILITY\s+(.+?)\s*$", re.I | re.M)


def detect_root(explicit: str | None) -> Path:
    candidates = []
    if explicit:
        candidates.append(Path(explicit))
    if os.environ.get("EPOCROOT"):
        candidates.append(Path(os.environ["EPOCROOT"]))
    candidates.extend([
        Path(r"C:\S60\devices\S60_3rd_FP2_SDK_v1.1"),
        Path(r"C:\Symbian\9.3"),
    ])
    for p in candidates:
        if (p / "epoc32" / "include").exists():
            return p
    raise SystemExit("SDK not found. Pass --sdk-root or set EPOCROOT.")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--sdk-root")
    ap.add_argument("--out", default="symbian-sdk-index.json")
    ap.add_argument("--symbol", help="Print matching headers and MMP files")
    args = ap.parse_args()
    root = detect_root(args.sdk_root)
    include = root / "epoc32" / "include"

    index = {"sdk_root": str(root), "headers": {}, "mmp": {}}
    needle = args.symbol.lower() if args.symbol else None
    matches = []

    for p in include.rglob("*.h"):
        try:
            text = p.read_text(encoding="latin-1", errors="ignore")
        except OSError:
            continue
        classes = sorted(set(CLASS_RE.findall(text)))
        methods = sorted(set(METHOD_RE.findall(text)))
        if classes or methods:
            index["headers"][str(p)] = {"classes": classes, "exported_methods": methods}
        if needle and needle in text.lower():
            matches.append(("header", str(p)))

    for p in root.rglob("*.mmp"):
        try:
            text = p.read_text(encoding="latin-1", errors="ignore")
        except OSError:
            continue
        libs = [m.strip() for m in LIB_RE.findall(text)]
        caps = [m.strip() for m in CAP_RE.findall(text)]
        index["mmp"][str(p)] = {"libraries": libs, "capabilities": caps}
        if needle and needle in text.lower():
            matches.append(("mmp", str(p)))

    Path(args.out).write_text(json.dumps(index, indent=2, ensure_ascii=False), encoding="utf-8")
    print(f"SDK: {root}")
    print(f"Indexed headers: {len(index['headers'])}")
    print(f"Indexed MMP files: {len(index['mmp'])}")
    print(f"Output: {args.out}")
    if needle:
        print(f"Matches for {args.symbol!r}:")
        for kind, path in matches:
            print(f"  {kind}: {path}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
