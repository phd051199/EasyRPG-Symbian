#!/usr/bin/env python3
"""Verify vendored EasyRPG sources against official release archives.

This tool never downloads or rewrites source files. It compares the files that
are already checked into third_party/ against caller-supplied official tarballs.
Use --require-complete once the whole upstream trees have been vendored.
"""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path, PurePosixPath
import sys
import tarfile
from typing import Dict, Iterable, Tuple


PROJECT_ROOT = Path(__file__).resolve().parent.parent

PACKAGES = {
    "player": {
        "vendor": PROJECT_ROOT / "third_party" / "easyrpg-player-0.5.0",
        "archive_root": PurePosixPath("easyrpg-player-0.5.0"),
        "archive_sha256": "21c9b95536fb6348e84b11ac8e5908688c6e61e9b4eb426247d546a44869241e",
    },
    "liblcf": {
        "vendor": PROJECT_ROOT / "third_party" / "liblcf-0.5.0",
        "archive_root": PurePosixPath("liblcf-0.5.0"),
        "archive_sha256": "5f77e40bac5b48ab3163c9628846825e44ae8b5ecb4367de2d55845b2ba48524",
    },
}


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def iter_vendor_files(root: Path) -> Iterable[Tuple[PurePosixPath, Path]]:
    if not root.is_dir():
        return
    for path in sorted(root.rglob("*")):
        if path.is_file():
            yield PurePosixPath(path.relative_to(root).as_posix()), path


def load_archive_files(path: Path, archive_root: PurePosixPath) -> Dict[PurePosixPath, bytes]:
    files: Dict[PurePosixPath, bytes] = {}
    prefix = archive_root.as_posix() + "/"
    with tarfile.open(path, "r:gz") as archive:
        for member in archive.getmembers():
            if not member.isfile() or not member.name.startswith(prefix):
                continue
            relative = PurePosixPath(member.name[len(prefix):])
            extracted = archive.extractfile(member)
            if extracted is None:
                raise RuntimeError(f"cannot read archive member: {member.name}")
            files[relative] = extracted.read()
    return files


def verify_package(name: str, archive_path: Path, require_complete: bool) -> bool:
    package = PACKAGES[name]
    vendor_root: Path = package["vendor"]
    archive_root: PurePosixPath = package["archive_root"]
    expected_archive_hash: str = package["archive_sha256"]

    if not archive_path.is_file():
        print(f"{name}: archive not found: {archive_path}", file=sys.stderr)
        return False

    actual_archive_hash = sha256_file(archive_path)
    if actual_archive_hash != expected_archive_hash:
        print(
            f"{name}: archive SHA-256 mismatch\n"
            f"  expected {expected_archive_hash}\n"
            f"  actual   {actual_archive_hash}",
            file=sys.stderr,
        )
        return False

    archive_files = load_archive_files(archive_path, archive_root)
    vendor_files = dict(iter_vendor_files(vendor_root))

    mismatches = []
    unknown = []
    for relative, path in vendor_files.items():
        original = archive_files.get(relative)
        if original is None:
            unknown.append(relative)
            continue
        local_hash = sha256_file(path)
        upstream_hash = sha256_bytes(original)
        if local_hash != upstream_hash:
            mismatches.append((relative, upstream_hash, local_hash))

    missing = sorted(set(archive_files) - set(vendor_files)) if require_complete else []

    if mismatches:
        print(f"{name}: {len(mismatches)} modified vendored file(s):", file=sys.stderr)
        for relative, upstream_hash, local_hash in mismatches[:30]:
            print(
                f"  {relative}\n"
                f"    upstream {upstream_hash}\n"
                f"    local    {local_hash}",
                file=sys.stderr,
            )
    if unknown:
        print(f"{name}: {len(unknown)} local file(s) absent from release archive:", file=sys.stderr)
        for relative in unknown[:30]:
            print(f"  {relative}", file=sys.stderr)
    if missing:
        print(f"{name}: {len(missing)} upstream file(s) not yet vendored:", file=sys.stderr)
        for relative in missing[:30]:
            print(f"  {relative}", file=sys.stderr)

    ok = not mismatches and not unknown and not missing
    checked = sum(1 for relative in vendor_files if relative in archive_files)
    if ok:
        suffix = " complete" if require_complete else " present-files"
        print(f"{name}: OK ({checked} vendored files match upstream;{suffix} verification)")
    else:
        print(
            f"{name}: FAILED (checked={checked}, mismatched={len(mismatches)}, "
            f"unknown={len(unknown)}, missing={len(missing)})",
            file=sys.stderr,
        )
    return ok


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--player-archive", type=Path)
    parser.add_argument("--liblcf-archive", type=Path)
    parser.add_argument(
        "--require-complete",
        action="store_true",
        help="also fail when any upstream release file is not present in third_party",
    )
    args = parser.parse_args()

    requested = []
    if args.player_archive:
        requested.append(("player", args.player_archive))
    if args.liblcf_archive:
        requested.append(("liblcf", args.liblcf_archive))
    if not requested:
        parser.error("provide --player-archive and/or --liblcf-archive")

    ok = True
    for name, archive_path in requested:
        ok = verify_package(name, archive_path.resolve(), args.require_complete) and ok
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
