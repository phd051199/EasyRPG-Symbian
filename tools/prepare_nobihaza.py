#!/usr/bin/env python3
"""Prepare NobihazaVH for the Symbian EasyRPG port.

The source game is kept untouched under .games/. Staging normalizes every path
component to Unicode NFC because the archive contains many decomposed Japanese
filenames while RPG Maker's CP932 database resolves to precomposed characters.
Symbian's filesystem is treated as case-insensitive, so collisions are rejected
before any copy starts.
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import sys
import unicodedata


REQUIRED = ("RPG_RT.ldb", "RPG_RT.lmt", "Map0001.lmu")
DEFAULT_EXCLUDED = {"RPG_RT.exe", "easyrpg_log.txt"}


def normalize_component(value: str) -> str:
    return unicodedata.normalize("NFC", value)


def normalized_relative(path: Path) -> Path:
    return Path(*(normalize_component(part) for part in path.parts))


def should_skip(relative: Path, include_save: bool) -> bool:
    if relative.name in DEFAULT_EXCLUDED:
        return True
    if not include_save and relative.name.lower().startswith("save") and relative.suffix.lower() == ".lsd":
        return True
    return False


def collect(source: Path, include_save: bool) -> list[tuple[Path, Path]]:
    result: list[tuple[Path, Path]] = []
    collision_map: dict[str, Path] = {}

    for current_root, dirs, files in os.walk(source):
        dirs.sort()
        files.sort()
        current = Path(current_root)
        for filename in files:
            src = current / filename
            relative = src.relative_to(source)
            if should_skip(relative, include_save):
                continue

            dst_relative = normalized_relative(relative)
            # Symbian/S60 file lookup is effectively case-insensitive for this
            # use case, so reject both Unicode-normalization and case collisions.
            key = "/".join(dst_relative.parts).casefold()
            previous = collision_map.get(key)
            if previous is not None and previous != relative:
                raise RuntimeError(
                    "staging collision after NFC/case normalization: "
                    f"{previous!s} <-> {relative!s} => {dst_relative!s}"
                )
            collision_map[key] = relative
            result.append((src, dst_relative))

    return result


def validate_source(source: Path) -> None:
    if not source.is_dir():
        raise RuntimeError(f"game directory not found: {source}")
    missing = [name for name in REQUIRED if not (source / name).is_file()]
    if missing:
        raise RuntimeError("not an RPG Maker 2000/2003 project; missing: " + ", ".join(missing))


def stage(source: Path, destination: Path, entries: list[tuple[Path, Path]]) -> tuple[int, int, int]:
    if source.resolve() == destination.resolve():
        raise RuntimeError("destination must not be the source game directory")

    normalized_count = 0
    byte_count = 0
    for src, relative in entries:
        original_relative = src.relative_to(source)
        if original_relative != relative:
            normalized_count += 1

        dst = destination / relative
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)
        byte_count += src.stat().st_size

    return len(entries), normalized_count, byte_count


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Validate/stage NobihazaVH for EasyRPG-Symbian (NFC-normalized paths)."
    )
    parser.add_argument(
        "--source",
        type=Path,
        default=Path(".games/NobihazaVH"),
        help="extracted game directory (default: .games/NobihazaVH)",
    )
    parser.add_argument(
        "--dest",
        type=Path,
        help="destination, e.g. /Volumes/E72/EasyRPG/Games/NobihazaVH",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="validate and report only; do not copy files",
    )
    parser.add_argument(
        "--include-save",
        action="store_true",
        help="include existing Save*.lsd files (disabled by default for a clean test)",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    source = args.source.resolve()

    try:
        validate_source(source)
        entries = collect(source, args.include_save)
        changed = sum(1 for src, rel in entries if src.relative_to(source) != rel)
        total_bytes = sum(src.stat().st_size for src, _ in entries)

        print(f"source: {source}")
        print(f"files: {len(entries)}")
        print(f"bytes: {total_bytes}")
        print(f"paths requiring NFC normalization: {changed}")
        print("engine markers: RPG_RT.ldb + RPG_RT.lmt + Map0001.lmu OK")

        if args.check:
            return 0
        if args.dest is None:
            raise RuntimeError("--dest is required unless --check is used")

        destination = args.dest.resolve()
        count, normalized_count, copied_bytes = stage(source, destination, entries)
        print(f"staged: {count} files / {copied_bytes} bytes")
        print(f"normalized: {normalized_count} paths")
        print(f"device game root: {destination}")
        return 0
    except (OSError, RuntimeError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
