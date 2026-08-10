# Local SDK and documentation corpus generators

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-15-local-corpus-generators-00-method-level-coverage-md"></a>
## Method-level coverage status

- Original corpus path: `15_LOCAL_CORPUS_GENERATORS/00_METHOD_LEVEL_COVERAGE.md`
- Consolidated reference: `14-local-corpus-generators.md#src-15-local-corpus-generators-00-method-level-coverage-md`

### Method-level coverage status

This packaged corpus intentionally separates three coverage levels:

1. **API-group level**
   - S60 Quick Reference name, published/deprecated release and public headers.
2. **Class level**
   - curated common classes with header, library, capability notes and lifecycle rules.
3. **Method level**
   - must be generated from the installed SDK headers and/or a legitimately obtained
     offline Developer Library because signatures differ across S60 editions and plug-ins.

The generator guides in this directory produce method-level Markdown from the user's
actual S60 3rd FP2 installation. This is more reliable than a static dump because it
captures API Plug-in, Eseries and device-specific additions.

---

<a id="src-15-local-corpus-generators-01-doxygen-headers-to-markdown-md"></a>
## Generate method-level Markdown from installed SDK headers

- Original corpus path: `15_LOCAL_CORPUS_GENERATORS/01_DOXYGEN_HEADERS_TO_MARKDOWN.md`
- Consolidated reference: `14-local-corpus-generators.md#src-15-local-corpus-generators-01-doxygen-headers-to-markdown-md`

### Generate method-level Markdown from installed SDK headers

This workflow reads the SDK you actually compile against.

#### 1. Copy the include tree

From Windows 7:

```bat
xcopy /E /I /H "%EPOCROOT%epoc32\include" C:\symbian-corpus\include
```

Run the same copy after installing API Plug-in/Eseries extensions so their headers are
included.

#### 2. Install and configure Doxygen

Create `Doxyfile`:

```text
PROJECT_NAME           = S60_3rd_FP2_SDK
OUTPUT_DIRECTORY       = output
INPUT                  = include
FILE_PATTERNS          = *.h *.hrh *.inl
RECURSIVE              = YES
EXTRACT_ALL             = YES
EXTRACT_PRIVATE         = NO
EXTRACT_STATIC          = YES
HIDE_UNDOC_MEMBERS      = NO
HIDE_UNDOC_CLASSES      = NO
GENERATE_HTML           = NO
GENERATE_XML            = YES
XML_OUTPUT              = xml
ENABLE_PREPROCESSING    = YES
MACRO_EXPANSION         = YES
EXPAND_ONLY_PREDEF      = NO
PREDEFINED              = IMPORT_C= \
                          EXPORT_C= \
                          GLDEF_C= \
                          LOCAL_C= \
                          NONSHARABLE_CLASS(x)="class x" \
                          NONSHARABLE_STRUCT(x)="struct x"
SKIP_FUNCTION_MACROS    = NO
QUIET                   = YES
WARN_IF_UNDOCUMENTED    = NO
```

Run:

```bat
doxygen Doxyfile
```

#### 3. Convert Doxygen XML to one Markdown file per class

```python
from pathlib import Path
import xml.etree.ElementTree as ET
import re, html, hashlib

XML = Path("output/xml")
OUT = Path("method_markdown")
OUT.mkdir(exist_ok=True)

def text(node):
    if node is None:
        return ""
    return " ".join("".join(node.itertext()).split())

def slug(s):
    return re.sub(r"[^a-zA-Z0-9_.-]+", "_", s).strip("_")

index = []

for f in XML.glob("class*.xml"):
    root = ET.parse(f).getroot()
    comp = root.find("compounddef")
    if comp is None:
        continue

    kind = comp.get("kind", "")
    name = text(comp.find("compoundname"))
    if not name:
        continue

    includes = [text(x) for x in comp.findall("includes")]
    brief = text(comp.find("briefdescription"))
    detail = text(comp.find("detaileddescription"))
    bases = [text(x) for x in comp.findall("basecompoundref")]

    lines = [
        "---",
        f"title: {name}",
        f"kind: {kind}",
        "source: installed_sdk_doxygen",
        "---",
        "",
        f"# `{name}`",
        "",
        f"- Kind: `{kind}`",
        f"- Headers: {', '.join(f'`{x}`' for x in includes) or 'unresolved'}",
        f"- Bases: {', '.join(f'`{x}`' for x in bases) or 'none listed'}",
        "",
        "## Description",
        "",
        brief or detail or "No documentation comment was present in the installed header.",
        "",
        "## Members",
        "",
    ]

    members = []
    for section in comp.findall("sectiondef"):
        section_kind = section.get("kind", "")
        for m in section.findall("memberdef"):
            mkind = m.get("kind", "")
            mname = text(m.find("name"))
            definition = text(m.find("definition"))
            args = text(m.find("argsstring"))
            mbrief = text(m.find("briefdescription"))
            mdetail = text(m.find("detaileddescription"))
            protection = m.get("prot", "")
            static = m.get("static", "no")
            const = m.get("const", "no")

            signature = (definition + args).strip()
            if not signature:
                signature = mname

            members.append({
                "section": section_kind,
                "kind": mkind,
                "name": mname,
                "signature": signature,
                "description": mbrief or mdetail,
                "protection": protection,
                "static": static,
                "const": const,
            })

    for m in members:
        lines += [
            f"### `{m['name']}`",
            "",
            f"```cpp\n{m['signature']}\n```",
            "",
            f"- Section: `{m['section']}`",
            f"- Kind: `{m['kind']}`",
            f"- Protection: `{m['protection']}`",
            f"- Static: `{m['static']}`",
            f"- Const: `{m['const']}`",
            "",
            m["description"] or "No documentation comment was present in the installed header.",
            "",
        ]

    body = "\n".join(lines)
    dest = OUT / f"{slug(name)}.md"
    dest.write_text(body, encoding="utf-8")
    index.append((name, dest.name, len(members), hashlib.sha256(body.encode()).hexdigest()))

index.sort(key=lambda x: x[0].lower())
idx = [
    "# Installed SDK class index",
    "",
    "| Class | File | Members | SHA-256 |",
    "|---|---|---:|---|",
]
for name, filename, count, sha in index:
    idx.append(f"| `{name}` | [{filename}]({filename}) | {count} | `{sha}` |")

(OUT / "INDEX.md").write_text("\n".join(idx) + "\n", encoding="utf-8")
print("generated classes:", len(index))
```

#### 4. Important limitations

- Doxygen sees declarations and comments present in headers; it does not discover the
  correct import library automatically.
- Some macros confuse the parser; add them to `PREDEFINED`.
- Internal headers may appear under `include\internal`; exclude them for third-party app
  training unless the project explicitly targets internal APIs.
- A declared API can still be absent on a specific firmware implementation.

---

<a id="src-15-local-corpus-generators-02-offline-developer-library-html-to-md-md"></a>
## Convert an offline Developer Library to Markdown

- Original corpus path: `15_LOCAL_CORPUS_GENERATORS/02_OFFLINE_DEVELOPER_LIBRARY_HTML_TO_MD.md`
- Consolidated reference: `14-local-corpus-generators.md#src-15-local-corpus-generators-02-offline-developer-library-html-to-md-md`

### Convert an offline Developer Library to Markdown

Use this only with documentation you obtained legitimately. The converter keeps one
Markdown file per HTML topic and records the source-relative path.

#### Dependencies

```bat
py -m pip install beautifulsoup4 markdownify lxml
```

#### Converter

```python
from pathlib import Path
from bs4 import BeautifulSoup
from markdownify import markdownify as md
import re, hashlib, json

SOURCE = Path(r"C:\symbian-docs\html")
OUT = Path(r"C:\symbian-docs\markdown")
OUT.mkdir(parents=True, exist_ok=True)

def clean_name(path):
    rel = path.relative_to(SOURCE).with_suffix("")
    parts = [re.sub(r"[^A-Za-z0-9_.-]+", "_", p) for p in rel.parts]
    return "__".join(parts) + ".md"

records = []

for html_file in SOURCE.rglob("*.htm*"):
    raw = html_file.read_text(encoding="utf-8", errors="ignore")
    soup = BeautifulSoup(raw, "lxml")

    for tag in soup(["script", "style", "noscript"]):
        tag.decompose()

    main = (
        soup.find(id="main")
        or soup.find(class_="main")
        or soup.find("body")
        or soup
    )

    title = ""
    if soup.title:
        title = soup.title.get_text(" ", strip=True)
    h1 = main.find("h1") if hasattr(main, "find") else None
    if h1:
        title = h1.get_text(" ", strip=True) or title
    title = title or html_file.stem

    markdown = md(str(main), heading_style="ATX", bullets="-")
    markdown = re.sub(r"\n{3,}", "\n\n", markdown).strip()

    rel_source = html_file.relative_to(SOURCE).as_posix()
    body = (
        "---\n"
        f"title: {json.dumps(title, ensure_ascii=False)}\n"
        "source_kind: offline_developer_library\n"
        f"source_path: {json.dumps(rel_source)}\n"
        "---\n\n"
        f"{markdown}\n"
    )

    out_file = OUT / clean_name(html_file)
    out_file.write_text(body, encoding="utf-8")
    records.append({
        "title": title,
        "source": rel_source,
        "file": out_file.name,
        "sha256": hashlib.sha256(body.encode()).hexdigest(),
    })

records.sort(key=lambda x: (x["title"].lower(), x["source"]))
index = [
    "# Offline Developer Library index",
    "",
    "| Title | Source | Markdown | SHA-256 |",
    "|---|---|---|---|",
]
for r in records:
    index.append(
        f"| {r['title'].replace('|', '\\|')} | `{r['source']}` | "
        f"[{r['file']}]({r['file']}) | `{r['sha256']}` |"
    )

(OUT / "INDEX.md").write_text("\n".join(index) + "\n", encoding="utf-8")
print("converted topics:", len(records))
```

#### Post-processing rules for agent training

- Keep class/member signature code blocks intact.
- Remove navigation-only pages and duplicate frames.
- Tag every chunk with platform/version.
- Do not merge S60 5th/Symbian^3 topics into the FP2 namespace.
- Prefer the installed header when HTML and compiler disagree.

---

<a id="src-15-local-corpus-generators-03-merge-dedup-and-validate-md"></a>
## 03 MERGE DEDUP AND VALIDATE

- Original corpus path: `15_LOCAL_CORPUS_GENERATORS/03_MERGE_DEDUP_AND_VALIDATE.md`
- Consolidated reference: `14-local-corpus-generators.md#src-15-local-corpus-generators-03-merge-dedup-and-validate-md`

### Merge, deduplicate and validate generated corpora

#### Deduplication priorities

When multiple files describe the same symbol:

1. installed FP2 header declaration;
2. matching FP2 class/member documentation;
3. matching SDK example;
4. API Plug-in documentation;
5. open-source implementation;
6. community article.

Do not discard conflicting records. Store the conflict and platform tag.

#### Validator

```python
from pathlib import Path
import re, hashlib, collections

ROOTS = [
    Path("method_markdown"),
    Path("developer_library_markdown"),
    Path("Symbian_OS_9.3_Carbide_2.7_Agent_Knowledge_V2"),
]

seen_hash = {}
symbols = collections.defaultdict(list)
problems = []

for root in ROOTS:
    if not root.exists():
        continue
    for p in root.rglob("*.md"):
        text = p.read_text(encoding="utf-8", errors="replace")
        sha = hashlib.sha256(text.encode()).hexdigest()

        if sha in seen_hash:
            problems.append(("duplicate", str(p), str(seen_hash[sha])))
        else:
            seen_hash[sha] = p

        for sym in re.findall(r"`([A-Z][A-Za-z0-9_:]+)`", text):
            symbols[sym].append(str(p))

        if "target:" not in text[:1000] and "source_kind:" not in text[:1000]:
            problems.append(("missing-platform-metadata", str(p), ""))

print("unique files:", len(seen_hash))
print("symbols:", len(symbols))
print("problems:", len(problems))

Path("VALIDATION_REPORT.md").write_text(
    "# Validation report\n\n" +
    "\n".join(f"- **{kind}** `{a}` `{b}`" for kind, a, b in problems) +
    "\n",
    encoding="utf-8",
)
```

#### Final acceptance checklist

- no non-Markdown files in the agent corpus;
- every file carries source/platform metadata;
- no method is labeled FP2 merely from an OS 9.3 page;
- plugin APIs are separated from base-SDK APIs;
- header/library/capability conflicts remain visible;
- generated code compiles in both WINSCW UDEB and GCCE UREL.
