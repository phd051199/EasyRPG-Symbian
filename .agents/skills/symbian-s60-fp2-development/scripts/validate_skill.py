#!/usr/bin/env python3
"""Validate this Agent Skill against core open-spec and hosted-skill constraints."""
from __future__ import annotations
from pathlib import Path
import re
import sys

MAX_FILES = 500
MAX_FILE_SIZE = 25 * 1024 * 1024
NAME_RE = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")
LINK_RE = re.compile(r"(?<!!)\[[^\]]+\]\(([^)]+)\)")


def parse_frontmatter(text: str):
    lines = text.splitlines()
    if not lines or lines[0].strip() != "---":
        raise ValueError("SKILL.md must start with YAML frontmatter")
    try:
        end = next(i for i in range(1, len(lines)) if lines[i].strip() == "---")
    except StopIteration:
        raise ValueError("Unclosed YAML frontmatter")
    data = {}
    for line in lines[1:end]:
        if line.startswith(" ") or not line.strip() or ":" not in line:
            continue
        key, value = line.split(":", 1)
        data[key.strip()] = value.strip().strip('"')
    return data


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    errors = []
    skills = [p for p in root.rglob("*") if p.is_file() and p.name.lower() == "skill.md"]
    if len(skills) != 1:
        errors.append(f"Expected exactly one SKILL.md, found {len(skills)}")
    else:
        try:
            fm = parse_frontmatter(skills[0].read_text(encoding="utf-8"))
            name = fm.get("name", "")
            desc = fm.get("description", "")
            if name != root.name:
                errors.append(f"name {name!r} must match directory {root.name!r}")
            if not NAME_RE.fullmatch(name) or len(name) > 64:
                errors.append("Invalid skill name")
            if not desc or len(desc) > 1024:
                errors.append("description must contain 1-1024 characters")
        except Exception as exc:
            errors.append(str(exc))

    files = [p for p in root.rglob("*") if p.is_file()]
    if len(files) > MAX_FILES:
        errors.append(f"File count {len(files)} exceeds {MAX_FILES}")
    for p in files:
        if p.stat().st_size > MAX_FILE_SIZE:
            errors.append(f"File too large: {p.relative_to(root)}")

    for p in root.rglob("*.md"):
        text = p.read_text(encoding="utf-8", errors="replace")
        in_fence = False
        for n, line in enumerate(text.splitlines(), 1):
            if line.lstrip().startswith(("```", "~~~")):
                in_fence = not in_fence
                continue
            if in_fence:
                continue
            for match in LINK_RE.finditer(line):
                target = match.group(1).strip().split()[0].strip("<>")
                if target.startswith(("http://", "https://", "mailto:", "#")):
                    continue
                path_part = target.split("#", 1)[0]
                if path_part and not (p.parent / path_part).exists():
                    errors.append(f"Broken local link {p.relative_to(root)}:{n} -> {target}")

    if errors:
        print("Validation failed:")
        for e in errors:
            print("-", e)
        return 1
    print(f"Validation passed: {len(files)} files, exactly one SKILL.md")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
