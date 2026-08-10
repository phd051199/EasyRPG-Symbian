#!/usr/bin/env python3
"""Vendor the complete liblcf 0.5.0 src/ tree from the pinned archive."""

from __future__ import annotations

import hashlib
import io
from pathlib import Path, PurePosixPath
import sys
import tarfile
import urllib.request


PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEST_ROOT = PROJECT_ROOT / "third_party" / "liblcf-0.5.0" / "src"
ARCHIVE_URL = "https://easyrpg.org/downloads/player/0.5.0/liblcf-0.5.0.tar.gz"
ARCHIVE_SHA256 = "5f77e40bac5b48ab3163c9628846825e44ae8b5ecb4367de2d55845b2ba48524"
ARCHIVE_PREFIX = PurePosixPath("liblcf-0.5.0/src")


def download_archive() -> bytes:
    print(f"Downloading {ARCHIVE_URL}")
    with urllib.request.urlopen(ARCHIVE_URL, timeout=60) as response:
        data = response.read()
    digest = hashlib.sha256(data).hexdigest()
    if digest != ARCHIVE_SHA256:
        raise RuntimeError(
            f"liblcf archive SHA-256 mismatch: expected {ARCHIVE_SHA256}, got {digest}"
        )
    print(f"Archive SHA-256 OK: {digest}")
    return data


def archive_sources(data: bytes) -> dict[PurePosixPath, bytes]:
    sources: dict[PurePosixPath, bytes] = {}
    with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as tar:
        for member in tar.getmembers():
            path = PurePosixPath(member.name)
            try:
                relative = path.relative_to(ARCHIVE_PREFIX)
            except ValueError:
                continue
            if not member.isfile() or not relative.parts:
                continue
            extracted = tar.extractfile(member)
            if extracted is None:
                raise RuntimeError(f"cannot read archive member: {member.name}")
            sources[relative] = extracted.read()
    if not sources:
        raise RuntimeError("official archive contained no src/ files")
    return sources


def main() -> int:
    try:
        archive = download_archive()
        sources = archive_sources(archive)

        mismatches: list[PurePosixPath] = []
        existing = 0
        for relative, expected in sources.items():
            destination = DEST_ROOT.joinpath(*relative.parts)
            if destination.exists():
                existing += 1
                if destination.read_bytes() != expected:
                    mismatches.append(relative)
        if mismatches:
            formatted = "\n".join(f"  {path}" for path in mismatches[:30])
            more = "" if len(mismatches) <= 30 else f"\n  ... {len(mismatches) - 30} more"
            raise RuntimeError(
                "refusing to overwrite liblcf files that differ from 0.5.0:\n"
                f"{formatted}{more}"
            )

        added = 0
        for relative, expected in sources.items():
            destination = DEST_ROOT.joinpath(*relative.parts)
            if destination.exists():
                continue
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(expected)
            added += 1

        print(
            f"liblcf src vendor OK: {len(sources)} files total, "
            f"{existing} already matched, {added} added"
        )
        return 0
    except (OSError, RuntimeError, tarfile.TarError) as exc:
        print(f"vendor_liblcf_src.py: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
