#!/usr/bin/env python3
import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys
import zipfile


SOURCE_SUFFIXES = {".ldb", ".lmt", ".lmu"}
SAVE_SUFFIXES = {".lsd"}
FIELDS = [
    "archive",
    "encoding",
    "source_files",
    "save_files",
    "save_loadable",
    "save_unreadable",
    "source_text_fields",
    "save_text_fields",
    "unique_texts",
    "characters",
    "squares",
    "sha256",
    "save_status_sha256",
]


def safe_name(name):
    stem = re.sub(r"[^A-Za-z0-9._-]+", "_", name).strip("_")
    digest = hashlib.sha1(name.encode("utf-8")).hexdigest()[:10]
    return (stem[:64] or "game") + "-" + digest


def extract_archive(archive, destination):
    shutil.rmtree(destination, ignore_errors=True)
    os.makedirs(destination, exist_ok=True)
    with zipfile.ZipFile(archive) as zf:
        for info in zf.infolist():
            normalized = info.filename.replace("\\", "/")
            if info.is_dir() or normalized.startswith("__MACOSX/"):
                continue
            zf.extract(info, destination)


def find_files(root, suffixes):
    result = []
    for base, _, names in os.walk(root):
        for name in names:
            if os.path.splitext(name)[1].lower() in suffixes:
                result.append(os.path.join(base, name))
    result.sort(key=lambda path: os.path.relpath(path, root).replace(os.sep, "/").lower())
    return result


def find_database(files):
    matches = [path for path in files if os.path.basename(path).lower() == "rpg_rt.ldb"]
    if not matches:
        raise RuntimeError("RPG_RT.ldb not found")
    matches.sort(key=lambda path: (path.count(os.sep), len(path)))
    return matches[0]


def run(binary, *args):
    proc = subprocess.run(
        [binary] + list(args),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    if proc.returncode != 0:
        raise RuntimeError(
            "command failed: {}\n{}{}".format(" ".join(args), proc.stdout, proc.stderr)
        )
    return proc.stdout.strip()


def run_dump(binary, encoding, files, batch_path, output_path):
    with open(batch_path, "w", encoding="utf-8", newline="\n") as batch:
        for source in files:
            batch.write(source + "\n")
    return subprocess.run(
        [binary, "--dump-batch", encoding, batch_path, output_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )


def command_failure(proc, encoding, batch_path, output_path):
    return "command failed: --dump-batch {} {} {}\n{}{}".format(
        encoding, batch_path, output_path, proc.stdout, proc.stderr
    )


def escape_tsv(value):
    output = []
    for char in value:
        codepoint = ord(char)
        if char == "\\":
            output.append("\\\\")
        elif char == "\t":
            output.append("\\t")
        elif char == "\r":
            output.append("\\r")
        elif char == "\n":
            output.append("\\n")
        elif codepoint < 0x20 or codepoint == 0x7F:
            output.append("\\x{:02x}".format(codepoint))
        else:
            output.append(char)
    return "".join(output)


def decode_hex(value, archive, source, ordinal):
    try:
        raw = bytes.fromhex(value)
        return raw.decode("utf-8", errors="strict")
    except (ValueError, UnicodeDecodeError) as exc:
        raise RuntimeError(
            "{} {} text #{} is not valid UTF-8: {}".format(
                archive, source, ordinal, exc
            )
        )


def consume_raw_corpus(raw_path, extracted, archive_name, kind, output, digest, unique):
    text_fields = 0
    characters = 0
    squares = 0
    with open(raw_path, "r", encoding="utf-8") as raw_input:
        header = raw_input.readline().rstrip("\n").split("\t")
        if header != ["source", "ordinal", "raw_hex", "utf8_hex"]:
            raise RuntimeError("Unexpected raw corpus format")
        for line in raw_input:
            source, ordinal_text, raw_hex, utf8_hex = line.rstrip("\n").split("\t")
            ordinal = int(ordinal_text)
            relative = os.path.relpath(source, extracted).replace(os.sep, "/")
            text = decode_hex(utf8_hex, archive_name, relative, ordinal)
            if "\ufffd" in text:
                raise RuntimeError(
                    "{} {} text #{} contains U+FFFD".format(
                        archive_name, relative, ordinal
                    )
                )
            squares += text.count("\u25a1")
            escaped = escape_tsv(text)
            normalized = "{}\t{}\t{}\t{}\t{}\n".format(
                kind, escape_tsv(relative), ordinal, raw_hex, escaped
            )
            output.write(normalized)
            digest.update(normalized.encode("utf-8"))
            unique.add(text)
            characters += len(text)
            text_fields += 1
    return text_fields, characters, squares


def collect_game(binary, archive, work_root):
    archive_name = os.path.basename(archive)
    stable_key = safe_name(archive_name)
    run_key = stable_key + "-{}".format(os.getpid())
    extracted = os.path.join(work_root, "extracted", run_key)
    corpus_root = os.path.join(work_root, "corpus")
    os.makedirs(corpus_root, exist_ok=True)
    extract_archive(archive, extracted)

    source_files = find_files(extracted, SOURCE_SUFFIXES)
    save_files = find_files(extracted, SAVE_SUFFIXES)
    database = find_database(source_files)
    game_dir = os.path.dirname(database)
    ini_path = os.path.join(game_dir, "RPG_RT.ini")
    ini_encoding = run(binary, "--ini-detect", ini_path).splitlines()[-1].strip()
    encoding = (
        ini_encoding
        if ini_encoding != "-"
        else run(binary, "--detect", database).splitlines()[-1].strip()
    )

    source_batch = os.path.join(work_root, run_key + ".source.batch.txt")
    source_raw = os.path.join(work_root, run_key + ".source.raw.tsv")
    source_proc = run_dump(binary, encoding, source_files, source_batch, source_raw)
    if source_proc.returncode != 0:
        raise RuntimeError(command_failure(source_proc, encoding, source_batch, source_raw))

    save_outputs = []
    save_loadable = 0
    save_unreadable = 0
    save_status = hashlib.sha256()
    for index, save_file in enumerate(save_files):
        relative = os.path.relpath(save_file, extracted).replace(os.sep, "/")
        save_batch = os.path.join(work_root, "{}.save{:03d}.batch.txt".format(run_key, index))
        save_raw = os.path.join(work_root, "{}.save{:03d}.raw.tsv".format(run_key, index))
        proc = run_dump(binary, encoding, [save_file], save_batch, save_raw)
        if proc.returncode == 0:
            save_loadable += 1
            save_outputs.append(save_raw)
            save_status.update((relative + "\tok\n").encode("utf-8"))
            continue

        # Old archives often ship stale/corrupt save slots. A structural load
        # failure is baseline data, not an encoding regression. Decode or
        # round-trip failures still fail the corpus immediately.
        if "FAIL load " in proc.stderr and "FAIL decode " not in proc.stderr and "FAIL semantic roundtrip " not in proc.stderr:
            save_unreadable += 1
            save_status.update((relative + "\tunreadable\n").encode("utf-8"))
            continue
        raise RuntimeError(command_failure(proc, encoding, save_batch, save_raw))

    corpus_path = os.path.join(corpus_root, stable_key + ".texts.tsv")
    digest = hashlib.sha256()
    unique = set()
    source_text_fields = 0
    save_text_fields = 0
    characters = 0
    squares = 0
    with open(corpus_path, "w", encoding="utf-8", newline="\n") as output:
        output.write("kind\tsource\tordinal\traw_hex\ttext\n")
        count, chars, boxes = consume_raw_corpus(
            source_raw, extracted, archive_name, "source", output, digest, unique
        )
        source_text_fields += count
        characters += chars
        squares += boxes
        for save_raw in save_outputs:
            count, chars, boxes = consume_raw_corpus(
                save_raw, extracted, archive_name, "save", output, digest, unique
            )
            save_text_fields += count
            characters += chars
            squares += boxes

    return {
        "archive": archive_name,
        "encoding": encoding,
        "source_files": len(source_files),
        "save_files": len(save_files),
        "save_loadable": save_loadable,
        "save_unreadable": save_unreadable,
        "source_text_fields": source_text_fields,
        "save_text_fields": save_text_fields,
        "unique_texts": len(unique),
        "characters": characters,
        "squares": squares,
        "sha256": digest.hexdigest(),
        "save_status_sha256": save_status.hexdigest(),
        "corpus": corpus_path,
    }


def load_manifest(path):
    result = {}
    with open(path, "r", encoding="utf-8") as handle:
        header = handle.readline().rstrip("\n").split("\t")
        if header != FIELDS:
            raise RuntimeError("Unexpected corpus manifest format")
        for line in handle:
            values = line.rstrip("\n").split("\t")
            row = dict(zip(FIELDS, values))
            result[row["archive"]] = row
    return result


def write_manifest(path, rows):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write("\t".join(FIELDS) + "\n")
        for row in rows:
            handle.write("\t".join(str(row[field]) for field in FIELDS) + "\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", required=True)
    parser.add_argument("--games", required=True)
    parser.add_argument("--work", required=True)
    parser.add_argument("--manifest", required=True)
    parser.add_argument("--update", action="store_true")
    args = parser.parse_args()

    archives = sorted(
        os.path.join(args.games, name)
        for name in os.listdir(args.games)
        if name.lower().endswith(".zip")
    )
    if not archives:
        raise RuntimeError("No game archives found")

    os.makedirs(args.work, exist_ok=True)
    rows = []
    collection_failures = 0
    for archive in archives:
        try:
            row = collect_game(args.binary, archive, args.work)
        except Exception as exc:
            print(
                "FAIL {}: {}".format(os.path.basename(archive), exc),
                file=sys.stderr,
            )
            collection_failures += 1
            continue
        rows.append(row)
        print(
            "{archive}: {encoding}, source={source_files}/{source_text_fields} text, "
            "save={save_loadable}/{save_files} loadable + {save_text_fields} text, "
            "chars={characters}, squares={squares}, sha256={sha256}".format(**row)
        )

    if collection_failures:
        return 1

    if args.update:
        write_manifest(args.manifest, rows)
        print("Updated {}".format(args.manifest))
        return 0

    if not os.path.isfile(args.manifest):
        raise RuntimeError("Missing manifest {}; run with --update once".format(args.manifest))
    expected = load_manifest(args.manifest)
    failures = 0
    current_names = {row["archive"] for row in rows}
    for row in rows:
        baseline = expected.get(row["archive"])
        if baseline is None:
            print("FAIL new archive missing from manifest: {}".format(row["archive"]), file=sys.stderr)
            failures += 1
            continue
        for field in FIELDS[1:]:
            if str(row[field]) != baseline[field]:
                print(
                    "FAIL {} {}: expected {}, got {}".format(
                        row["archive"], field, baseline[field], row[field]
                    ),
                    file=sys.stderr,
                )
                failures += 1
    for archive in sorted(set(expected) - current_names):
        print("FAIL manifest archive missing from games/: {}".format(archive), file=sys.stderr)
        failures += 1
    if failures:
        return 1
    print("ALL CORPUS PASS ({} archives)".format(len(rows)))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except Exception as exc:
        print("FAIL: {}".format(exc), file=sys.stderr)
        sys.exit(1)
