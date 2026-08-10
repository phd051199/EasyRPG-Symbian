#!/usr/bin/env python3
"""Search consolidated Symbian skill references and print ranked excerpts."""
from __future__ import annotations
import argparse
from pathlib import Path
import re


def tokenize(text: str) -> list[str]:
    return [t.lower() for t in re.findall(r"[A-Za-z0-9_:+.\\/-]+", text) if len(t) > 1]


def sections(text: str):
    lines = text.splitlines()
    starts = [i for i, line in enumerate(lines) if re.match(r"^#{1,6}\s+", line)]
    if not starts:
        yield 1, "Document", text
        return
    for pos, start in enumerate(starts):
        end = starts[pos + 1] if pos + 1 < len(starts) else len(lines)
        heading = re.sub(r"^#{1,6}\s+", "", lines[start]).strip()
        body = "\n".join(lines[start:end]).strip()
        yield start + 1, heading, body


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("query", help="Symbols or concepts to search")
    ap.add_argument("--top", type=int, default=10)
    ap.add_argument("--context", type=int, default=1400, help="Maximum excerpt characters")
    args = ap.parse_args()

    root = Path(__file__).resolve().parents[1] / "references"
    phrase = args.query.lower().strip()
    terms = tokenize(args.query)
    results = []

    for path in root.glob("*.md"):
        text = path.read_text(encoding="utf-8", errors="replace")
        for line_no, heading, body in sections(text):
            hay = body.lower()
            score = 0
            if phrase and phrase in hay:
                score += 80
            for term in terms:
                count = hay.count(term)
                score += min(count, 8) * 5
                if term in heading.lower():
                    score += 20
                if re.search(rf"`{re.escape(term)}`", hay, flags=re.I):
                    score += 12
            if score:
                results.append((score, path.name, line_no, heading, body))

    results.sort(key=lambda x: (-x[0], x[1], x[2]))
    for i, (score, filename, line_no, heading, body) in enumerate(results[:args.top], 1):
        excerpt = body[:args.context]
        if len(body) > args.context:
            excerpt += "\n…"
        print(f"\n[{i}] score={score} references/{filename}:{line_no} — {heading}\n")
        print(excerpt)
    if not results:
        print("No matches. Try exact class/header names or fewer terms.")
        return 1
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
