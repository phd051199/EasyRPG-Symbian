from __future__ import annotations

import argparse
from dataclasses import dataclass
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unicodedata
import zlib

MAX_CENTRAL_DIRECTORY = 2 * 1024 * 1024
MAX_ARCHIVE_SIZE = 0x7FFFFFFF
MAX_ENTRIES = 0xFFFE
FLAG_ENCRYPTED = 0x0001
FLAG_UTF8 = 0x0800
METHOD_STORED = 0
EXTRA_UNICODE_PATH = 0x7075
EXTRA_WINZIP_AES = 0x9901
LOCAL_SIGNATURE = 0x04034B50
CENTRAL_SIGNATURE = 0x02014B50
EOCD_SIGNATURE = 0x06054B50


@dataclass
class Entry:
    made_by: int
    needed: int
    flags: int
    method: int
    mod_time: int
    mod_date: int
    crc: int
    compressed_size: int
    uncompressed_size: int
    central_name: bytes
    central_extra: bytes
    comment: bytes
    disk_start: int
    internal_attr: int
    external_attr: int
    local_offset: int
    local_needed: int
    local_flags: int
    local_method: int
    local_time: int
    local_date: int
    local_crc: int
    local_compressed_size: int
    local_uncompressed_size: int
    local_name: bytes
    local_extra: bytes


@dataclass
class Archive:
    entries: list[Entry]
    comment: bytes


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Remove ZIP encryption and store every existing archive entry without compression while preserving file names and file contents."
    )
    parser.add_argument("source", type=Path)
    parser.add_argument("-o", "--output", type=Path)
    parser.add_argument("--password")
    parser.add_argument("--password-file", type=Path)
    return parser.parse_args()


def find_7zip() -> str:
    for candidate in ("7zz", "7z"):
        path = shutil.which(candidate)
        if path:
            return path
    raise RuntimeError("7zz/7z is required")


def read_password(args: argparse.Namespace) -> str | None:
    if args.password is not None and args.password_file is not None:
        raise RuntimeError("Use only one of --password or --password-file")
    if args.password_file is not None:
        return args.password_file.read_text(encoding="utf-8").rstrip("\r\n")
    return args.password


def find_eocd(handle, file_size: int) -> tuple[int, bytes]:
    tail_size = min(file_size, 22 + 65535)
    handle.seek(file_size - tail_size)
    tail = handle.read(tail_size)
    for offset in range(tail_size - 22, -1, -1):
        if tail[offset:offset + 4] != b"PK\x05\x06":
            continue
        comment_length = struct.unpack_from("<H", tail, offset + 20)[0]
        if offset + 22 + comment_length == tail_size:
            return file_size - tail_size + offset, tail[offset + 22:offset + 22 + comment_length]
    raise RuntimeError("EOCD not found")


def parse_archive(path: Path) -> Archive:
    file_size = path.stat().st_size
    with path.open("rb") as handle:
        eocd_offset, archive_comment = find_eocd(handle, file_size)
        handle.seek(eocd_offset + 4)
        fields = handle.read(18)
        if len(fields) != 18:
            raise RuntimeError("Truncated EOCD")
        disk, central_disk, disk_entries, entries, central_size, central_offset, comment_length = struct.unpack("<HHHHIIH", fields)
        if disk != 0 or central_disk != 0 or disk_entries != entries:
            raise RuntimeError("Multi-disk ZIP is not supported")
        if entries == 0xFFFF or central_size == 0xFFFFFFFF or central_offset == 0xFFFFFFFF:
            raise RuntimeError("Zip64 is not supported")
        if entries > MAX_ENTRIES:
            raise RuntimeError(f"Too many archive entries: {entries}")
        if comment_length != len(archive_comment):
            raise RuntimeError("Invalid ZIP comment")
        if central_offset + central_size > eocd_offset:
            raise RuntimeError("Invalid central directory bounds")
        handle.seek(central_offset)
        central = handle.read(central_size)
        if len(central) != central_size:
            raise RuntimeError("Truncated central directory")

        result: list[Entry] = []
        cursor = 0
        for _ in range(entries):
            if cursor + 46 > len(central) or struct.unpack_from("<I", central, cursor)[0] != CENTRAL_SIGNATURE:
                raise RuntimeError("Invalid central directory entry")
            made_by, needed, flags, method, mod_time, mod_date, crc, compressed_size, uncompressed_size, name_length, extra_length, entry_comment_length, disk_start, internal_attr, external_attr, local_offset = struct.unpack_from(
                "<HHHHHHIIIHHHHHII", central, cursor + 4
            )
            record_size = 46 + name_length + extra_length + entry_comment_length
            if cursor + record_size > len(central):
                raise RuntimeError("Truncated central directory record")
            name_start = cursor + 46
            central_name = central[name_start:name_start + name_length]
            extra_start = name_start + name_length
            central_extra = central[extra_start:extra_start + extra_length]
            comment = central[extra_start + extra_length:extra_start + extra_length + entry_comment_length]
            if disk_start != 0 or local_offset == 0xFFFFFFFF or compressed_size == 0xFFFFFFFF or uncompressed_size == 0xFFFFFFFF:
                raise RuntimeError("Zip64 or multi-disk entry is not supported")

            current = handle.tell()
            handle.seek(local_offset)
            local_header = handle.read(30)
            if len(local_header) != 30 or struct.unpack_from("<I", local_header, 0)[0] != LOCAL_SIGNATURE:
                raise RuntimeError("Invalid local header")
            local_needed, local_flags, local_method, local_time, local_date, local_crc, local_compressed_size, local_uncompressed_size, local_name_length, local_extra_length = struct.unpack_from(
                "<HHHHHIIIHH", local_header, 4
            )
            local_name = handle.read(local_name_length)
            local_extra = handle.read(local_extra_length)
            if len(local_name) != local_name_length or len(local_extra) != local_extra_length:
                raise RuntimeError("Truncated local header")
            handle.seek(current)

            result.append(
                Entry(
                    made_by,
                    needed,
                    flags,
                    method,
                    mod_time,
                    mod_date,
                    crc,
                    compressed_size,
                    uncompressed_size,
                    central_name,
                    central_extra,
                    comment,
                    disk_start,
                    internal_attr,
                    external_attr,
                    local_offset,
                    local_needed,
                    local_flags,
                    local_method,
                    local_time,
                    local_date,
                    local_crc,
                    local_compressed_size,
                    local_uncompressed_size,
                    local_name,
                    local_extra,
                )
            )
            cursor += record_size

        if cursor != len(central):
            raise RuntimeError("Central directory size mismatch")
        return Archive(result, archive_comment)


def iter_extra(extra: bytes):
    cursor = 0
    while cursor < len(extra):
        if cursor + 4 > len(extra):
            raise RuntimeError("Malformed ZIP extra field")
        field_id, size = struct.unpack_from("<HH", extra, cursor)
        end = cursor + 4 + size
        if end > len(extra):
            raise RuntimeError("Malformed ZIP extra field")
        yield field_id, extra[cursor + 4:end]
        cursor = end


def strip_encryption_extra(extra: bytes) -> bytes:
    output = bytearray()
    for field_id, payload in iter_extra(extra):
        if field_id == EXTRA_WINZIP_AES:
            continue
        output += struct.pack("<HH", field_id, len(payload))
        output += payload
    return bytes(output)


def decode_name(raw_name: bytes, extra: bytes, flags: int) -> str:
    for field_id, payload in iter_extra(extra):
        if field_id != EXTRA_UNICODE_PATH or len(payload) < 5 or payload[0] != 1:
            continue
        stored_crc = struct.unpack_from("<I", payload, 1)[0]
        if stored_crc == zlib.crc32(raw_name) & 0xFFFFFFFF:
            return payload[5:].decode("utf-8")
    if flags & FLAG_UTF8:
        return raw_name.decode("utf-8")
    try:
        return raw_name.decode("utf-8")
    except UnicodeDecodeError:
        return raw_name.decode("cp932")


def safe_relative_name(entry: Entry) -> str:
    name = decode_name(entry.central_name, entry.central_extra, entry.flags).replace("\\", "/")
    while name.startswith("./"):
        name = name[2:]
    parts = name.split("/")
    if not name or name.startswith("/") or any(part == ".." for part in parts):
        raise RuntimeError(f"Unsafe archive path: {name}")
    return name


def is_directory(entry: Entry) -> bool:
    return entry.central_name.endswith((b"/", b"\\"))


def extract_archive(source: Path, destination: Path, password: str | None, encrypted: bool) -> None:
    if encrypted and password is None:
        raise RuntimeError("Input ZIP is encrypted. Pass --password or --password-file.")
    command = [find_7zip(), "x", "-y", f"-o{destination}"]
    if password is not None:
        command.append(f"-p{password}")
    command.append(str(source))
    result = subprocess.run(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )
    if result.returncode != 0:
        raise RuntimeError(result.stdout.strip() or f"7-Zip extraction failed with code {result.returncode}")


def extracted_files(root: Path) -> dict[str, Path]:
    files: dict[str, Path] = {}
    for path in root.rglob("*"):
        if not path.is_file() or path.is_symlink():
            continue
        relative = path.relative_to(root).as_posix()
        key = unicodedata.normalize("NFC", relative)
        if key in files:
            raise RuntimeError(f"Duplicate extracted path: {relative}")
        files[key] = path
    return files


def resolve_extracted_path(files: dict[str, Path], entry: Entry) -> Path:
    relative = safe_relative_name(entry).rstrip("/")
    key = unicodedata.normalize("NFC", relative)
    path = files.get(key)
    if path is None:
        raise RuntimeError(f"Extracted file not found: {relative}")
    return path


def file_crc_size(path: Path) -> tuple[int, int]:
    crc = 0
    size = 0
    with path.open("rb") as handle:
        while True:
            block = handle.read(1024 * 1024)
            if not block:
                break
            crc = zlib.crc32(block, crc)
            size += len(block)
    return crc & 0xFFFFFFFF, size


def copy_file(source: Path, destination) -> None:
    with source.open("rb") as handle:
        shutil.copyfileobj(handle, destination, length=1024 * 1024)


def write_archive(source_archive: Archive, extracted_root: Path, output: Path) -> None:
    files = extracted_files(extracted_root)
    output.parent.mkdir(parents=True, exist_ok=True)
    temp_output = output.with_name(f".{output.name}.tmp")
    if temp_output.exists():
        temp_output.unlink()

    central_records: list[tuple[Entry, int, int, int, int, bytes, bytes]] = []
    try:
        with temp_output.open("wb") as handle:
            for entry in source_archive.entries:
                directory = is_directory(entry)
                payload_path = None
                if directory:
                    crc = 0
                    size = 0
                else:
                    payload_path = resolve_extracted_path(files, entry)
                    crc, size = file_crc_size(payload_path)
                    if size != entry.uncompressed_size:
                        raise RuntimeError(f"File size changed during extraction: {safe_relative_name(entry)}")
                    if entry.crc != 0 and crc != entry.crc:
                        raise RuntimeError(f"File content changed during extraction: {safe_relative_name(entry)}")

                flags = entry.flags & FLAG_UTF8
                local_extra = strip_encryption_extra(entry.local_extra)
                central_extra = strip_encryption_extra(entry.central_extra)
                local_offset = handle.tell()
                if local_offset > 0xFFFFFFFF:
                    raise RuntimeError("Archive exceeds 32-bit offset limit")
                handle.write(
                    struct.pack(
                        "<IHHHHHIIIHH",
                        LOCAL_SIGNATURE,
                        20,
                        flags,
                        METHOD_STORED,
                        entry.local_time,
                        entry.local_date,
                        crc,
                        size,
                        size,
                        len(entry.local_name),
                        len(local_extra),
                    )
                )
                handle.write(entry.local_name)
                handle.write(local_extra)
                if payload_path is not None:
                    copy_file(payload_path, handle)
                central_records.append((entry, flags, crc, size, local_offset, central_extra, local_extra))

            central_offset = handle.tell()
            for entry, flags, crc, size, local_offset, central_extra, _ in central_records:
                handle.write(
                    struct.pack(
                        "<IHHHHHHIIIHHHHHII",
                        CENTRAL_SIGNATURE,
                        entry.made_by,
                        20,
                        flags,
                        METHOD_STORED,
                        entry.mod_time,
                        entry.mod_date,
                        crc,
                        size,
                        size,
                        len(entry.central_name),
                        len(central_extra),
                        len(entry.comment),
                        0,
                        entry.internal_attr,
                        entry.external_attr,
                        local_offset,
                    )
                )
                handle.write(entry.central_name)
                handle.write(central_extra)
                handle.write(entry.comment)

            central_size = handle.tell() - central_offset
            if central_size > MAX_CENTRAL_DIRECTORY:
                raise RuntimeError(f"Central directory exceeds {MAX_CENTRAL_DIRECTORY} bytes")
            if handle.tell() > MAX_ARCHIVE_SIZE:
                raise RuntimeError(f"Archive exceeds {MAX_ARCHIVE_SIZE} bytes")
            count = len(central_records)
            handle.write(
                struct.pack(
                    "<IHHHHIIH",
                    EOCD_SIGNATURE,
                    0,
                    0,
                    count,
                    count,
                    central_size,
                    central_offset,
                    len(source_archive.comment),
                )
            )
            handle.write(source_archive.comment)

        verify_output(source_archive, temp_output)
        os.replace(temp_output, output)
    finally:
        if temp_output.exists():
            temp_output.unlink()


def verify_output(source_archive: Archive, output: Path) -> None:
    if output.stat().st_size > MAX_ARCHIVE_SIZE:
        raise RuntimeError("Output exceeds 32-bit archive size limit")
    result = parse_archive(output)
    if result.comment != source_archive.comment:
        raise RuntimeError("Archive comment changed")
    if len(result.entries) != len(source_archive.entries):
        raise RuntimeError("Archive entry count changed")
    for source, target in zip(source_archive.entries, result.entries):
        if target.central_name != source.central_name or target.local_name != source.local_name:
            raise RuntimeError("Archive file name changed")
        if target.uncompressed_size != source.uncompressed_size:
            raise RuntimeError(f"Archive file size changed: {safe_relative_name(source)}")
        if target.method != METHOD_STORED or target.local_method != METHOD_STORED:
            raise RuntimeError(f"Archive entry is compressed: {safe_relative_name(source)}")
        if (target.flags & FLAG_ENCRYPTED) != 0 or (target.local_flags & FLAG_ENCRYPTED) != 0:
            raise RuntimeError(f"Archive entry is still encrypted: {safe_relative_name(source)}")
        if any(field_id == EXTRA_WINZIP_AES for field_id, _ in iter_extra(target.central_extra)):
            raise RuntimeError(f"AES metadata remains: {safe_relative_name(source)}")
        if any(field_id == EXTRA_WINZIP_AES for field_id, _ in iter_extra(target.local_extra)):
            raise RuntimeError(f"AES metadata remains: {safe_relative_name(source)}")
    test = subprocess.run(
        [find_7zip(), "t", str(output)],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )
    if test.returncode != 0:
        raise RuntimeError(test.stdout.strip() or "Output ZIP validation failed")


def default_output(source: Path) -> Path:
    return source.with_name(f"{source.stem}.store.zip")


def main() -> int:
    args = parse_args()
    source = args.source.expanduser().resolve()
    if not source.is_file():
        raise RuntimeError(f"ZIP source not found: {source}")
    output = (args.output or default_output(source)).expanduser().resolve()
    password = read_password(args)
    source_archive = parse_archive(source)
    encrypted = any((entry.flags & FLAG_ENCRYPTED) != 0 for entry in source_archive.entries)
    with tempfile.TemporaryDirectory(prefix="easyrpg-repack-") as temp:
        extracted = Path(temp)
        extract_archive(source, extracted, password, encrypted)
        write_archive(source_archive, extracted, output)
    print(f"output={output}")
    print(f"entries={len(source_archive.entries)}")
    print(f"size={output.stat().st_size}")
    print("profile=store/no-encryption/raw-names-preserved/content-preserved")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (RuntimeError, OSError, subprocess.SubprocessError, UnicodeError, struct.error) as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(1)
