#!/usr/bin/env python3
"""Clean and package the local Nobihaza test project for Symbian ZIP VFS.

The source folder is intentionally treated as game data rather than as a PC
runtime installation. Known desktop/emulator artifacts and stale save data are
removed, archive paths are normalized to Unicode NFC, and the output is a plain
ZIP32 archive that CZipVfs can mount without a password.
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path, PurePosixPath
import unicodedata
import zipfile


JUNK_RELATIVE_PATHS = (
    Path(".DS_Store"),
    Path("easyrpg_log.txt"),
    Path("RPG_RT.exe"),
    Path("Save15.lsd"),
    Path("gohufont.fon"),
    Path("Title") / "Thumbs.db",
)

# Symbian package policy: store every member without an outer DEFLATE layer.
# CZipVfs can then seek/read exact game bytes directly from RFile. This avoids
# raw-DEFLATE compatibility/CPU cost on the S60 FP2 zlib 1.1.3 runtime and also
# makes future streaming audio possible without inflating a whole ZIP member.
# PNG/XYZ/MP3 already contain their own compression where applicable.
STORE_ALL_MEMBERS = True


def normalize_archive_component(value: str) -> str:
    return unicodedata.normalize("NFC", value)


def normalized_arcname(root_name: str, relative: Path) -> str:
    parts = [normalize_archive_component(root_name)]
    parts.extend(normalize_archive_component(part) for part in relative.parts)
    return PurePosixPath(*parts).as_posix()


def remove_known_junk(game_dir: Path) -> tuple[int, int]:
    removed_count = 0
    removed_bytes = 0
    for relative in JUNK_RELATIVE_PATHS:
        path = game_dir / relative
        if not path.exists():
            continue
        if not path.is_file():
            raise RuntimeError(f"refusing to remove non-file junk candidate: {path}")
        removed_bytes += path.stat().st_size
        path.unlink()
        removed_count += 1
    return removed_count, removed_bytes


def collect_files(game_dir: Path, root_name: str) -> list[tuple[Path, str]]:
    files: list[tuple[Path, str]] = []
    normalized_keys: dict[str, Path] = {}

    for path in sorted(game_dir.rglob("*"), key=lambda p: p.as_posix()):
        if not path.is_file():
            continue
        relative = path.relative_to(game_dir)
        arcname = normalized_arcname(root_name, relative)
        key = arcname.casefold()
        previous = normalized_keys.get(key)
        if previous is not None:
            raise RuntimeError(
                "NFC/case-insensitive archive collision: "
                f"{previous.relative_to(game_dir)} <-> {relative} -> {arcname}"
            )
        normalized_keys[key] = path
        files.append((path, arcname))

    required = {
        f"{normalize_archive_component(root_name)}/RPG_RT.ldb".casefold(),
        f"{normalize_archive_component(root_name)}/RPG_RT.lmt".casefold(),
    }
    missing = required.difference(normalized_keys)
    if missing:
        raise RuntimeError(f"not an RPG Maker 2000 project; missing: {sorted(missing)}")
    return files


def pack(game_dir: Path, output: Path, root_name: str) -> tuple[int, int, int]:
    files = collect_files(game_dir, root_name)
    output.parent.mkdir(parents=True, exist_ok=True)

    # Write to a sibling temporary archive and atomically replace the old one
    # only after the complete ZIP has been finalized successfully.
    temp_output = output.with_suffix(output.suffix + ".tmp")
    if temp_output.exists():
        temp_output.unlink()

    source_bytes = 0
    stored_count = 0
    deflated_count = 0
    try:
        with zipfile.ZipFile(
            temp_output,
            mode="w",
            allowZip64=False,
        ) as archive:
            for path, arcname in files:
                source_bytes += path.stat().st_size
                if STORE_ALL_MEMBERS:
                    compress_type = zipfile.ZIP_STORED
                    compresslevel = None
                    stored_count += 1
                else:
                    compress_type = zipfile.ZIP_DEFLATED
                    compresslevel = 6
                    deflated_count += 1

                archive.write(
                    path,
                    arcname=arcname,
                    compress_type=compress_type,
                    compresslevel=compresslevel,
                )

        os.replace(temp_output, output)
    finally:
        if temp_output.exists():
            temp_output.unlink()

    return len(files), source_bytes, stored_count + deflated_count


def validate_archive(output: Path) -> tuple[int, int]:
    with zipfile.ZipFile(output, "r") as archive:
        bad = archive.testzip()
        if bad is not None:
            raise RuntimeError(f"ZIP CRC/decompression validation failed at {bad}")
        infos = archive.infolist()
        if not infos:
            raise RuntimeError("archive is empty")
        encrypted = [info.filename for info in infos if info.flag_bits & 0x1]
        if encrypted:
            raise RuntimeError(f"archive unexpectedly contains encrypted entries: {encrypted[:3]}")
        non_stored = [
            info.filename for info in infos if info.compress_type != zipfile.ZIP_STORED
        ]
        if non_stored:
            raise RuntimeError(
                f"Symbian archive unexpectedly contains DEFLATE entries: {non_stored[:3]}"
            )
        zip64 = [
            info.filename
            for info in infos
            if info.file_size >= 0xFFFFFFFF
            or info.compress_size >= 0xFFFFFFFF
            or info.header_offset >= 0xFFFFFFFF
        ]
        if zip64:
            raise RuntimeError(f"archive unexpectedly requires ZIP64: {zip64[:3]}")
        return len(infos), sum(info.file_size for info in infos)


def main() -> int:
    project_root = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--game-dir",
        type=Path,
        default=project_root / ".games" / "NobihazaVH",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=project_root / ".games" / "NobihazaVH.zip",
    )
    parser.add_argument("--root-name", default="NobihazaVH")
    parser.add_argument(
        "--keep-junk",
        action="store_true",
        help="package without deleting the known PC/emulator artifacts",
    )
    args = parser.parse_args()

    game_dir = args.game_dir.resolve()
    output = args.output.resolve()
    if not game_dir.is_dir():
        raise SystemExit(f"game directory not found: {game_dir}")

    removed_count = 0
    removed_bytes = 0
    if not args.keep_junk:
        removed_count, removed_bytes = remove_known_junk(game_dir)

    files, source_bytes, _ = pack(game_dir, output, args.root_name)
    validated_files, validated_bytes = validate_archive(output)
    if validated_files != files or validated_bytes != source_bytes:
        raise RuntimeError("ZIP validation totals differ from source totals")

    archive_bytes = output.stat().st_size
    print(
        "Clean Nobihaza ZIP OK: "
        f"removed={removed_count} ({removed_bytes} bytes) "
        f"files={files} source={source_bytes} zip={archive_bytes} "
        f"output={output}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
