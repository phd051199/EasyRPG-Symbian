#!/usr/bin/env python3
import re
import sys

register = r"(?:r(?:1[0-5]|[0-9])|ip|lr|sp)"
mul_pattern = re.compile(
    rf"^(\s*)mul([a-z]{{0,2}})(\s+)({register}),\s*({register}),\s*({register})(\s*(?:@.*)?)$"
)
mla_pattern = re.compile(
    rf"^(\s*)mla([a-z]{{0,2}})(\s+)({register}),\s*({register}),\s*({register}),\s*({register})(\s*(?:@.*)?)$"
)


def normalize(line):
    match = mul_pattern.match(line)
    if match:
        indent, suffix, gap, rd, rm, rs, tail = match.groups()
        if rd == rm and rd != rs:
            return f"{indent}mul{suffix}{gap}{rd}, {rs}, {rm}{tail}"
        return line
    match = mla_pattern.match(line)
    if match:
        indent, suffix, gap, rd, rm, rs, rn, tail = match.groups()
        if rd == rm and rd != rs:
            return f"{indent}mla{suffix}{gap}{rd}, {rs}, {rm}, {rn}{tail}"
    return line


def main():
    if len(sys.argv) != 3:
        return 2
    with open(sys.argv[1], "r", encoding="utf-8") as source:
        lines = source.read().splitlines()
    with open(sys.argv[2], "w", encoding="utf-8", newline="\n") as target:
        for line in lines:
            target.write(normalize(line))
            target.write("\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
