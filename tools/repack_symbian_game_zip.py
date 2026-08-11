from __future__ import annotations

import argparse
import mmap
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zipfile

MAX_CENTRAL_DIRECTORY = 2 * 1024 * 1024
MAX_ARCHIVE_SIZE = 0x7FFFFFFF
MAX_ENTRIES = 0xFFFE
MAX_PATH_CHARS = 256


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Repack an RPG Maker 2000/2003 project into the conservative ZIP profile used by NobihazaVH on EasyRPG Symbian."
    )
    parser.add_argument("source", type=Path)
    parser.add_argument("-o", "--output", type=Path)
    parser.add_argument("--root-name")
    parser.add_argument("--password")
    parser.add_argument("--password-file", type=Path)
    parser.add_argument("--full-package", action="store_true")
    return parser.parse_args()


def find_7zip() -> str:
    for candidate in ("7zz", "7z"):
        path = shutil.which(candidate)
        if path:
            return path
    raise RuntimeError("7zz/7z is required for ZIP input. Install p7zip or 7-Zip first.")


def read_password(args: argparse.Namespace) -> str | None:
    if args.password is not None and args.password_file is not None:
        raise RuntimeError("Use only one of --password or --password-file")
    if args.password_file is not None:
        return args.password_file.read_text(encoding="utf-8").rstrip("\r\n")
    return args.password


def archive_is_encrypted(seven_zip: str, source: Path) -> bool:
    result = subprocess.run(
        [seven_zip, "l", "-slt", str(source)],
        check=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )
    return any(line.strip() == "Encrypted = +" for line in result.stdout.splitlines())


def extract_archive(source: Path, destination: Path, password: str | None) -> None:
    seven_zip = find_7zip()
    if archive_is_encrypted(seven_zip, source) and password is None:
        raise RuntimeError("Input ZIP is encrypted. Pass --password or --password-file.")
    command = [seven_zip, "x", "-y", f"-o{destination}", str(source)]
    if password is not None:
        command.append(f"-p{password}")
    result = subprocess.run(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )
    if result.returncode != 0:
        raise RuntimeError(result.stdout.strip() or f"7-Zip extraction failed with code {result.returncode}")


def find_project_root(base: Path) -> Path:
    candidates = []
    for database in base.rglob("RPG_RT.ldb"):
        root = database.parent
        if (root / "RPG_RT.lmt").is_file():
            candidates.append(root)
    unique = sorted({item.resolve() for item in candidates}, key=lambda item: (len(item.parts), str(item)))
    if not unique:
        raise RuntimeError("No RPG Maker project root containing RPG_RT.ldb and RPG_RT.lmt was found")
    shallowest_depth = len(unique[0].parts)
    shallowest = [item for item in unique if len(item.parts) == shallowest_depth]
    if len(shallowest) != 1:
        names = ", ".join(str(item) for item in shallowest)
        raise RuntimeError(f"Multiple RPG Maker project roots were found: {names}")
    return shallowest[0]


def normalize_full_package(data: bytes) -> bytes:
    lines = data.splitlines(keepends=True)
    output = []
    found = False
    pattern = re.compile(br"^[ \t]*Full(?:FlagPackage|PackageFlag)[ \t]*=", re.IGNORECASE)
    newline = b"\r\n" if b"\r\n" in data else b"\n"
    for line in lines:
        body = line.rstrip(b"\r\n")
        ending = line[len(body):]
        if pattern.match(body):
            if not found:
                output.append(b"FullPackageFlag=1" + (ending or newline))
                found = True
            continue
        output.append(line)
    result = b"".join(output)
    if not found:
        if result and not result.endswith((b"\n", b"\r")):
            result += newline
        if not result:
            result = b"[RPG_RT]" + newline
        result += b"FullPackageFlag=1" + newline
    return result


def default_output(source: Path) -> Path:
    if source.is_dir():
        return source.with_suffix(".zip")
    return source.with_name(f"{source.stem}.symbian.zip")


def safe_root_name(value: str) -> str:
    name = value.strip().strip("/\\")
    if not name or name in {".", ".."} or "/" in name or "\\" in name:
        raise RuntimeError("Root name must be one directory name")
    return name


def iter_project_files(project_root: Path) -> list[Path]:
    files = [path for path in project_root.rglob("*") if path.is_file()]
    files.sort(key=lambda path: path.relative_to(project_root).as_posix())
    return files


def zip_info_for_file(source: Path, arcname: str) -> zipfile.ZipInfo:
    stat_result = source.stat()
    local_time = __import__("time").localtime(stat_result.st_mtime)
    year = min(max(local_time.tm_year, 1980), 2107)
    info = zipfile.ZipInfo(
        arcname,
        (year, local_time.tm_mon, local_time.tm_mday, local_time.tm_hour, local_time.tm_min, local_time.tm_sec),
    )
    info.create_system = 3
    info.create_version = 20
    info.extract_version = 20
    info.compress_type = zipfile.ZIP_STORED
    info.flag_bits = 0
    info.external_attr = ((stat_result.st_mode & 0xFFFF) << 16)
    info.extra = b""
    info.comment = b""
    return info


def write_payload(archive: zipfile.ZipFile, source: Path, arcname: str, data: bytes | None) -> None:
    info = zip_info_for_file(source, arcname)
    if data is not None:
        archive.writestr(info, data, compress_type=zipfile.ZIP_STORED)
        return
    with source.open("rb") as input_file, archive.open(info, "w", force_zip64=False) as output_file:
        shutil.copyfileobj(input_file, output_file, length=1024 * 1024)


def build_archive(project_root: Path, output: Path, root_name: str, full_package: bool) -> None:
    files = iter_project_files(project_root)
    if len(files) > MAX_ENTRIES:
        raise RuntimeError(f"Too many files for the Symbian ZIP reader: {len(files)}")
    output.parent.mkdir(parents=True, exist_ok=True)
    temp_output = output.with_name(f".{output.name}.tmp")
    if temp_output.exists():
        temp_output.unlink()
    try:
        with zipfile.ZipFile(temp_output, "w", compression=zipfile.ZIP_STORED, allowZip64=False) as archive:
            for source in files:
                relative = source.relative_to(project_root).as_posix()
                arcname = f"{root_name}/{relative}"
                if len(arcname) > MAX_PATH_CHARS:
                    raise RuntimeError(f"Archive path exceeds {MAX_PATH_CHARS} characters: {arcname}")
                data = None
                if full_package and relative.casefold() == "rpg_rt.ini":
                    data = normalize_full_package(source.read_bytes())
                write_payload(archive, source, arcname, data)
        verify_archive(temp_output, root_name)
        os.replace(temp_output, output)
    finally:
        if temp_output.exists():
            temp_output.unlink()


def find_eocd(data: bytes) -> int:
    minimum = max(0, len(data) - 22 - 65535)
    for offset in range(len(data) - 22, minimum - 1, -1):
        if data[offset:offset + 4] != b"PK\x05\x06":
            continue
        comment_length = struct.unpack_from("<H", data, offset + 20)[0]
        if offset + 22 + comment_length == len(data):
            return offset
    raise RuntimeError("EOCD not found")


def decode_name(raw: bytes, flags: int) -> str:
    if flags & 0x0800:
        return raw.decode("utf-8")
    return raw.decode("cp437")


def verify_archive(path: Path, root_name: str) -> None:
    size = path.stat().st_size
    if size > MAX_ARCHIVE_SIZE:
        raise RuntimeError(f"Archive exceeds Symbian TInt file size limit: {size} bytes")
    with path.open("rb") as archive_file, mmap.mmap(archive_file.fileno(), 0, access=mmap.ACCESS_READ) as data:
        eocd = find_eocd(data)
        disk, central_disk, disk_entries, entries, central_size, central_offset = struct.unpack_from("<HHHHII", data, eocd + 4)
        if disk != 0 or central_disk != 0 or disk_entries != entries:
            raise RuntimeError("Multi-disk ZIP is not supported")
        if entries >= 0xFFFF or central_size >= 0xFFFFFFFF or central_offset >= 0xFFFFFFFF:
            raise RuntimeError("Zip64 is not supported")
        if central_size > MAX_CENTRAL_DIRECTORY:
            raise RuntimeError(f"Central directory exceeds {MAX_CENTRAL_DIRECTORY} bytes")
        if central_offset + central_size > eocd:
            raise RuntimeError("Invalid central directory bounds")
        cursor = central_offset
        core = set()
        prefix = f"{root_name}/"
        for _ in range(entries):
            if data[cursor:cursor + 4] != b"PK\x01\x02":
                raise RuntimeError("Invalid central directory entry")
            made_by, needed, flags, method = struct.unpack_from("<HHHH", data, cursor + 4)
            compressed_size, uncompressed_size = struct.unpack_from("<II", data, cursor + 20)
            name_length, extra_length, comment_length, start_disk = struct.unpack_from("<HHHH", data, cursor + 28)
            local_offset = struct.unpack_from("<I", data, cursor + 42)[0]
            raw_name = data[cursor + 46:cursor + 46 + name_length]
            name = decode_name(raw_name, flags)
            if made_by & 0xFF != 20 or needed != 20:
                raise RuntimeError(f"Unsupported ZIP version in {name}")
            if flags & ~0x0800:
                raise RuntimeError(f"Unsupported ZIP flags 0x{flags:04x} in {name}")
            if method != 0:
                raise RuntimeError(f"Entry is not stored: {name}")
            if extra_length != 0 or comment_length != 0:
                raise RuntimeError(f"Entry contains extra metadata: {name}")
            if start_disk != 0 or compressed_size == 0xFFFFFFFF or uncompressed_size == 0xFFFFFFFF or local_offset == 0xFFFFFFFF:
                raise RuntimeError(f"Zip64 or multi-disk metadata in {name}")
            if name.endswith("/"):
                raise RuntimeError(f"Directory entry is not allowed: {name}")
            if not name.startswith(prefix):
                raise RuntimeError(f"Entry is outside root directory {root_name}: {name}")
            if len(name) > MAX_PATH_CHARS:
                raise RuntimeError(f"Path exceeds {MAX_PATH_CHARS} characters: {name}")
            relative = name[len(prefix):]
            if relative.casefold() in {"rpg_rt.ldb", "rpg_rt.lmt", "rpg_rt.ini"}:
                core.add(relative.casefold())
            if data[local_offset:local_offset + 4] != b"PK\x03\x04":
                raise RuntimeError(f"Invalid local header for {name}")
            local_needed, local_flags, local_method = struct.unpack_from("<HHH", data, local_offset + 4)
            local_name_length, local_extra_length = struct.unpack_from("<HH", data, local_offset + 26)
            local_raw_name = data[local_offset + 30:local_offset + 30 + local_name_length]
            if local_needed != 20 or local_flags != flags or local_method != 0 or local_extra_length != 0 or local_raw_name != raw_name:
                raise RuntimeError(f"Local header does not match VH profile for {name}")
            cursor += 46 + name_length + extra_length + comment_length
        if cursor != central_offset + central_size:
            raise RuntimeError("Central directory size mismatch")
        required = {"rpg_rt.ldb", "rpg_rt.lmt"}
        if not required.issubset(core):
            raise RuntimeError("RPG_RT.ldb or RPG_RT.lmt is missing from the project root")
    with zipfile.ZipFile(path, "r") as archive:
        bad_file = archive.testzip()
        if bad_file is not None:
            raise RuntimeError(f"CRC validation failed: {bad_file}")


def main() -> int:
    args = parse_args()
    source = args.source.expanduser().resolve()
    if not source.exists():
        raise RuntimeError(f"Source not found: {source}")
    password = read_password(args)
    output = (args.output or default_output(source)).expanduser().resolve()
    root_name = safe_root_name(args.root_name or source.stem if source.is_file() else args.root_name or source.name)
    if source.is_dir():
        project_root = find_project_root(source)
        build_archive(project_root, output, root_name, args.full_package)
    else:
        with tempfile.TemporaryDirectory(prefix="easyrpg-symbian-zip-") as temp:
            extracted = Path(temp)
            extract_archive(source, extracted, password)
            project_root = find_project_root(extracted)
            build_archive(project_root, output, root_name, args.full_package)
    with zipfile.ZipFile(output, "r") as archive:
        entries = len(archive.infolist())
    print(f"output={output}")
    print(f"root={root_name}/")
    print(f"entries={entries}")
    print(f"size={output.stat().st_size}")
    print("profile=VH-compatible store/no-extra/no-directory/no-zip64")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (RuntimeError, OSError, subprocess.SubprocessError, zipfile.BadZipFile) as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(1)
