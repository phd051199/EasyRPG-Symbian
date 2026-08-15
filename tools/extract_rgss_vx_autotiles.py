#!/usr/bin/env python3
from pathlib import Path
import argparse
import hashlib
import re
import sys
import tarfile

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "src/rgss/core/RgssVxAutotileTables.h"
UPSTREAM_VERSION = "2.4.2"
UPSTREAM_SHA256 = "4c2df25e30139678307a8d072c325dc94c01ebc024c47d51eee17dd6f2ee1aac"
AUTOTILES_SUFFIX = "/src/display/autotilesvx.cpp"
ATLAS_SUFFIX = "/src/display/gl/tileatlasvx.cpp"


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--stdout", action="store_true")
    parser.add_argument("--source-tar", type=Path)
    return parser.parse_args()


def read_member_by_suffix(archive, suffix):
    matches = [member for member in archive.getmembers()
               if member.isfile() and member.name.endswith(suffix)]
    if len(matches) != 1:
        raise SystemExit(f"expected one {suffix} in mkxp-z tarball, found {len(matches)}")
    fileobj = archive.extractfile(matches[0])
    if fileobj is None:
        raise SystemExit(f"cannot read {matches[0].name}")
    return fileobj.read().decode("utf-8", errors="strict"), matches[0].name


def load_from_tar(path):
    if not path.exists():
        raise SystemExit(f"mkxp-z source tarball not found: {path}")
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    if digest != UPSTREAM_SHA256:
        raise SystemExit(f"mkxp-z tarball SHA-256 mismatch: {digest}")
    with tarfile.open(path, "r:gz") as archive:
        autotiles, autotiles_name = read_member_by_suffix(archive, AUTOTILES_SUFFIX)
        atlas, atlas_name = read_member_by_suffix(archive, ATLAS_SUFFIX)
    return autotiles, atlas, autotiles_name, atlas_name


def load_from_tree():
    roots = [
        ROOT / "build/work/third-party" / f"mkxp-z-{UPSTREAM_VERSION}",
        ROOT / "third_party" / f"mkxp-z-{UPSTREAM_VERSION}",
    ]
    for root in roots:
        autotiles_path = root / "src/display/autotilesvx.cpp"
        atlas_path = root / "src/display/gl/tileatlasvx.cpp"
        if autotiles_path.is_file() and atlas_path.is_file():
            return (autotiles_path.read_text(), atlas_path.read_text(),
                    autotiles_path.relative_to(ROOT).as_posix(),
                    atlas_path.relative_to(ROOT).as_posix())
    return None


def rect_array(text, name):
    match = re.search(
        r"(?:extern\s+)?(?:static\s+)?const\s+StaticRect\s+" +
        re.escape(name) + r"\[\]\s*=\s*\{(.*?)\};",
        text, re.S)
    if not match:
        raise SystemExit(f"missing {name} in mkxp-z source")
    rects = []
    for values in re.findall(
            r"\{\s*(-?[\d.]+)f?\s*,\s*(-?[\d.]+)f?\s*,\s*"
            r"(-?[\d.]+)f?\s*,\s*(-?[\d.]+)f?\s*\}", match.group(1)):
        rects.append(tuple(float(value) for value in values))
    return rects


def quarter(rect):
    x, y, _, _ = rect
    return int(x // 16.0), int(y // 16.0)


def grouped_quarters(rects, patterns):
    if len(rects) != patterns * 4:
        raise SystemExit(f"unexpected autotile rect count: {len(rects)} != {patterns * 4}")
    return [[quarter(rects[pattern * 4 + quadrant]) for quadrant in range(4)]
            for pattern in range(patterns)]


def waterfall_quarters(rects):
    if len(rects) != 8:
        raise SystemExit(f"unexpected waterfall rect count: {len(rects)} != 8")
    result = []
    for pattern in range(4):
        left = quarter(rects[pattern * 2])
        right = quarter(rects[pattern * 2 + 1])
        result.append([
            left,
            right,
            (left[0], left[1] + 1),
            (right[0], right[1] + 1),
        ])
    return result


def emit_table(lines, name, table):
    lines.append(f"static const signed char {name}[{len(table)}][4][2] = {{")
    for index, quarters in enumerate(table):
        flat = tuple(value for pair in quarters for value in pair)
        suffix = "," if index + 1 < len(table) else ""
        lines.append("    {{%d,%d},{%d,%d},{%d,%d},{%d,%d}}%s" %
                     (flat + (suffix,)))
    lines += ["};", ""]


def generate(autotiles_text, atlas_text, autotiles_name, atlas_name):
    floor = grouped_quarters(rect_array(autotiles_text, "autotileVXRectsA"), 48)
    wall = grouped_quarters(rect_array(autotiles_text, "autotileVXRectsB"), 16)
    waterfall = waterfall_quarters(rect_array(atlas_text, "autotileVXRectsC"))

    lines = [
        "#ifndef RGSS_VX_AUTOTILE_TABLES_H",
        "#define RGSS_VX_AUTOTILE_TABLES_H",
        "",
        f"/* Generated from mkxp-z {UPSTREAM_VERSION} source.",
        f" * Upstream tarball SHA-256: {UPSTREAM_SHA256}",
        f" * Floor/wall: {autotiles_name}",
        f" * Waterfall: {atlas_name}",
        " * StaticRect coordinates are converted to 16x16 quarter indices.",
        " */",
        "namespace rgss {",
        "",
    ]
    emit_table(lines, "KVxFloorAutotileTable", floor)
    emit_table(lines, "KVxWallAutotileTable", wall)
    emit_table(lines, "KVxWaterfallAutotileTable", waterfall)
    lines += ["} // namespace rgss", "", "#endif", ""]
    return "\n".join(lines)


def validate_existing(text):
    if UPSTREAM_SHA256 not in text:
        raise SystemExit("RGSS VX autotile header lacks mkxp-z provenance SHA-256")
    expected = {
        "KVxFloorAutotileTable": (48, 384),
        "KVxWallAutotileTable": (16, 128),
        "KVxWaterfallAutotileTable": (4, 32),
    }
    for name, (patterns, values) in expected.items():
        match = re.search(
            re.escape(name) + rf"\[{patterns}\]\[4\]\[2\]\s*=\s*\{{(.*?)\}};",
            text, re.S)
        if not match:
            raise SystemExit(f"RGSS VX autotile header missing {name}")
        numbers = re.findall(r"(?<![A-Za-z_])-?\d+", match.group(1))
        if len(numbers) != values:
            raise SystemExit(f"RGSS VX autotile header has malformed {name}: {len(numbers)} values")


def main():
    args = parse_args()
    source = load_from_tar(args.source_tar) if args.source_tar else load_from_tree()

    if source is None:
        if args.stdout or not args.check:
            raise SystemExit(
                "full mkxp-z source is not vendored; pass --source-tar <mkxp-z-2.4.2.tar.gz>")
        if not OUTPUT.exists():
            raise SystemExit("RGSS VX autotile header is missing")
        validate_existing(OUTPUT.read_text())
        print(OUTPUT.relative_to(ROOT))
        return

    generated = generate(*source)
    if args.stdout:
        sys.stdout.write(generated)
        return
    if args.check:
        if not OUTPUT.exists() or OUTPUT.read_text() != generated:
            raise SystemExit(
                "RGSS VX autotile table header is stale; regenerate with "
                "tools/extract_rgss_vx_autotiles.py --source-tar <mkxp-z-2.4.2.tar.gz>")
    else:
        OUTPUT.write_text(generated)
    print(OUTPUT.relative_to(ROOT))


if __name__ == "__main__":
    main()
