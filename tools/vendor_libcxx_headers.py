#!/usr/bin/env python3
"""Vendor selected libc++ 4.0.1 headers from the official LLVM archive.

This helper is intentionally narrow: it downloads the documented upstream
archive, verifies its SHA-256, and copies requested headers byte-for-byte into
the existing dependency-driven vendor tree. Existing files are never silently
overwritten when they differ from the release archive.
"""

from __future__ import annotations

import argparse
import hashlib
import io
from pathlib import Path, PurePosixPath
import sys
import tarfile
import urllib.request


PROJECT_ROOT = Path(__file__).resolve().parents[1]
VENDOR_INCLUDE = PROJECT_ROOT / "third_party" / "libcxx-4.0.1" / "include"
ARCHIVE_URL = "https://releases.llvm.org/4.0.1/libcxx-4.0.1.src.tar.xz"
ARCHIVE_SHA256 = "520a1171f272c9ff82f324d5d89accadcec9bc9f3c78de11f5575cdb99accc4c"
ARCHIVE_PREFIX = PurePosixPath("libcxx-4.0.1.src/include")
DEFAULT_HEADERS = ("algorithm", "memory", "string", "vector")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--replace-mismatch",
        action="store_true",
        help="replace an existing header only when it differs from the verified release archive",
    )
    parser.add_argument(
        "headers",
        nargs="*",
        default=DEFAULT_HEADERS,
        help="Header paths relative to libc++ include/ (default: %(default)s)",
    )
    return parser.parse_args()


def normalize_header(value: str) -> PurePosixPath:
    path = PurePosixPath(value)
    if path.is_absolute() or not path.parts or any(part in ("", ".", "..") for part in path.parts):
        raise ValueError(f"invalid header path: {value!r}")
    return path


def download_archive() -> bytes:
    print(f"Downloading {ARCHIVE_URL}")
    with urllib.request.urlopen(ARCHIVE_URL, timeout=60) as response:
        data = response.read()
    digest = hashlib.sha256(data).hexdigest()
    if digest != ARCHIVE_SHA256:
        raise RuntimeError(
            f"libc++ archive SHA-256 mismatch: expected {ARCHIVE_SHA256}, got {digest}"
        )
    print(f"Archive SHA-256 OK: {digest}")
    return data


def read_members(archive: bytes, headers: list[PurePosixPath]) -> dict[PurePosixPath, bytes]:
    result: dict[PurePosixPath, bytes] = {}
    with tarfile.open(fileobj=io.BytesIO(archive), mode="r:xz") as tar:
        for header in headers:
            member_name = str(ARCHIVE_PREFIX / header)
            try:
                member = tar.getmember(member_name)
            except KeyError as exc:
                raise RuntimeError(f"header not present in official archive: {header}") from exc
            if not member.isfile():
                raise RuntimeError(f"archive member is not a regular file: {member_name}")
            extracted = tar.extractfile(member)
            if extracted is None:
                raise RuntimeError(f"cannot read archive member: {member_name}")
            result[header] = extracted.read()
    return result


def vendor_header(header: PurePosixPath, expected: bytes, replace_mismatch: bool) -> None:
    destination = VENDOR_INCLUDE.joinpath(*header.parts)
    if destination.exists():
        current = destination.read_bytes()
        if current == expected:
            print(f"MATCH {header} ({len(current)} bytes)")
            return
        if not replace_mismatch:
            raise RuntimeError(
                f"refusing to overwrite modified/mismatched vendored header: {destination}"
            )
        destination.write_bytes(expected)
        print(f"REPLACE {header} ({len(current)} -> {len(expected)} bytes)")
        return

    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(expected)
    print(f"ADD   {header} ({len(expected)} bytes)")


def main() -> int:
    args = parse_args()
    try:
        headers = [normalize_header(value) for value in args.headers]
        if len(set(headers)) != len(headers):
            raise ValueError("duplicate header path")
        archive = download_archive()
        members = read_members(archive, headers)
        for header in headers:
            vendor_header(header, members[header], args.replace_mismatch)
    except (OSError, RuntimeError, ValueError, tarfile.TarError) as exc:
        print(f"vendor_libcxx_headers.py: {exc}", file=sys.stderr)
        return 1

    print(f"Vendored {len(headers)} libc++ 4.0.1 header(s) from the verified official archive.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
