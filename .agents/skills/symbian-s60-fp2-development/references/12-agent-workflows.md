# Agent workflows for API verification and implementation

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-13-agent-workflows-answer-template-md"></a>
## ANSWER TEMPLATE

- Original corpus path: `13_AGENT_WORKFLOWS/ANSWER_TEMPLATE.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-answer-template-md`

### Agent answer template for Symbian API questions

```markdown
## Compatibility verdict
- Target: S60 3rd Edition FP2 / Symbian OS 9.3
- Status: verified base SDK | plugin required | device extension | unavailable | local check
- Evidence: Quick Reference / class page / installed header / SDK example

## Build metadata
- Header:
- Library:
- Capability:
- UID/resource requirements:

## Ownership and asynchronous model
- Owner:
- Cleanup:
- Leaves:
- Callback:
- Cancel:

## Minimal code
...

## MMP/resource changes
...

## FP2 caveats
...

## Verification
- WINSCW:
- GCCE:
- device:
```

Never omit the compatibility verdict when the question involves a class from another
S60 edition.

---

<a id="src-13-agent-workflows-capability-resolution-md"></a>
## CAPABILITY RESOLUTION

- Original corpus path: `13_AGENT_WORKFLOWS/CAPABILITY_RESOLUTION.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-capability-resolution-md`

### Capability resolution

Capabilities attach to protected operations and server policies, not merely to a header.

#### Workflow

1. List exact methods used.
2. Read each member's capability line.
3. Check protected file paths, contact/message/calendar access and network operations.
4. Union the required capabilities.
5. Remove capabilities not justified by a call.
6. Rebuild and test installation/signing on device.

#### Common categories

- `NetworkServices`: normal IP networking.
- `LocalServices`: local links such as selected Bluetooth operations.
- `ReadUserData` / `WriteUserData`: contacts, messages, calendar and other user data.
- `Location`: position requests.
- `UserEnvironment`: selected camera/audio/user-environment operations.
- stronger capabilities may require signing unavailable to ordinary self-signed apps.

Do not use a huge capability list to “make it work”; that usually makes signing fail.

---

<a id="src-13-agent-workflows-compile-probe-pattern-md"></a>
## COMPILE PROBE PATTERN

- Original corpus path: `13_AGENT_WORKFLOWS/COMPILE_PROBE_PATTERN.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-compile-probe-pattern-md`

### Minimal compile/link probe pattern

Use one API per probe. This prevents unrelated Avkon/resource errors from hiding the
actual availability result.

#### `probe.mmp`

```text
TARGET          probe.exe
TARGETTYPE      exe
UID             0x100039CE 0xE00000F0
SECUREID        0xE00000F0
CAPABILITY      None

SOURCEPATH      .
SOURCE          probe.cpp
SYSTEMINCLUDE   \epoc32\include

LIBRARY         euser.lib
; Add only the library under test
```

#### `probe.cpp`

```cpp
#include <e32base.h>
#include <TARGET_HEADER.h>

GLDEF_C TInt E32Main()
{
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup)
        return KErrNoMemory;

    TRAPD(err,
        // Construct or reference one symbol from the target API.
    );

    delete cleanup;
    __UHEAP_MARKEND;
    return err;
}
```

#### Result interpretation

- Header missing: base SDK/plug-in mismatch or wrong include.
- Compile symbol missing: class/method is not declared for this SDK configuration.
- Linker unresolved: wrong/missing import library or API unavailable in target build.
- Emulator success only: still build GCCE and run on device.

---

<a id="src-13-agent-workflows-header-to-library-resolution-md"></a>
## HEADER TO LIBRARY RESOLUTION

- Original corpus path: `13_AGENT_WORKFLOWS/HEADER_TO_LIBRARY_RESOLUTION.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-header-to-library-resolution-md`

### Resolve header to import library

The Quick Reference gives public headers, not always libraries.

Preferred evidence order:

1. Exact class/member page: `Location` and `Link against`.
2. Official SDK example `.mmp`.
3. Source component `.mmp` from a matching Symbian/S60 repository.
4. Installed documentation index.
5. Minimal linker probe.

Never determine a library from its filename alone when multiple components expose
similar names.

#### Evidence record

```markdown
class: CExample
header: example.h
library: example.lib
evidence:
  - class_page
  - sdk_example: examples/example/group/example.mmp
sdk_builds:
  - winscw_udeb: pass
  - gcce_urel: pass
```

---

<a id="src-13-agent-workflows-porting-newer-symbian-code-to-fp2-md"></a>
## PORTING NEWER SYMBIAN CODE TO FP2

- Original corpus path: `13_AGENT_WORKFLOWS/PORTING_NEWER_SYMBIAN_CODE_TO_FP2.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-porting-newer-symbian-code-to-fp2-md`

### Port newer S60/Symbian code to FP2

#### Triage every error

| Error | First question |
|---|---|
| missing header | Was it introduced after FP2 or delivered by a plug-in? |
| undefined enum in `.rss` | Is the resource identifier from S60 5th/Symbian^3? |
| missing method | Was the method added to the class later? |
| unresolved symbol | Is the library absent, wrong or plugin-only? |
| capability/install error | Did the port introduce protected operations? |

#### Safe strategy

- Reduce to a minimal FP2 compile probe.
- Search the FP2 header, not a modern web snippet.
- Compare class members and overloads.
- Replace touch/layout APIs with keypad/non-touch equivalents.
- Replace unavailable multimedia convenience methods with the FP2-supported sequence.
- Gate device-specific behavior with feature discovery.

---

<a id="src-13-agent-workflows-rag-ingestion-profile-md"></a>
## RAG INGESTION PROFILE

- Original corpus path: `13_AGENT_WORKFLOWS/RAG_INGESTION_PROFILE.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-rag-ingestion-profile-md`

### RAG ingestion profile

#### Recommended chunking

- Keep YAML frontmatter with each chunk.
- Chunk by H2 section, 450–900 tokens.
- Preserve code fences with their preceding metadata.
- Give `00_AUDIT`, API matrices and class matrices higher retrieval priority.
- Do not merge plug-in files into base-SDK facts.

#### Retrieval filters

```text
target = "S60 3rd Edition Feature Pack 2"
verification in [
  "verified_quick_reference",
  "verified_header",
  "verified_class_page",
  "verified_installed_sdk"
]
```

Allow `plugin_v1_2_listed` only when the query mentions the API Plug-in or the installed
SDK scan confirms it.

#### Corpus counts generated in this build

- S60 Quick Reference API groups: 121
- FP2 plug-in v1.2 candidates: 48
- common class guides: 75

---

<a id="src-13-agent-workflows-sdk-local-scan-md"></a>
## Scan an installed S60 FP2 SDK

- Original corpus path: `13_AGENT_WORKFLOWS/SDK_LOCAL_SCAN.md`
- Consolidated reference: `12-agent-workflows.md#src-13-agent-workflows-sdk-local-scan-md`

### Scan an installed S60 FP2 SDK

The local SDK is the final compile-time authority.

#### Command Prompt inventory

```bat
set SDKROOT=%EPOCROOT%epoc32
dir /s /b "%SDKROOT%\include\*.h" > headers.txt
dir /s /b "%SDKROOT%\release\*.lib" > libraries.txt
dir /s /b "%SDKROOT%\*.mmp" > mmp_files.txt
dir /s /b "%SDKROOT%\*.rss" > resources.txt
```

#### Search a class or method

```bat
findstr /s /n /i /c:"class CVideoPlayerUtility" "%SDKROOT%\include\*.h"
findstr /s /n /i /c:"SetVideoRectL" "%SDKROOT%\include\*.h"
findstr /s /n /i /c:"mediaclientvideo.lib" C:\S60\devices\*.mmp
```

#### Python indexer

Save this as `scan_symbian_sdk.py` on the Windows 7 VM:

```python
from pathlib import Path
import json, re, os

epocroot = Path(os.environ.get("EPOCROOT", r"C:\S60\devices\S60_3rd_FP2_SDK_v1.1"))
root = epocroot / "epoc32"
patterns = {
    "class": re.compile(r"\b(class|struct)\s+(?:EXPORT_C\s+)?([A-Z]\w+)"),
    "include": re.compile(r"^\s*#\s*include\s*[<\"]([^>\"]+)", re.M),
    "library": re.compile(r"^\s*LIBRARY\s+(.+)$", re.M | re.I),
    "capability": re.compile(r"^\s*CAPABILITY\s+(.+)$", re.M | re.I),
}
index = {"headers": {}, "mmp": {}}

for p in (root / "include").rglob("*.h"):
    try:
        text = p.read_text(encoding="latin-1", errors="ignore")
    except OSError:
        continue
    classes = sorted({m.group(2) for m in patterns["class"].finditer(text)})
    if classes:
        index["headers"][str(p)] = classes

for p in epocroot.rglob("*.mmp"):
    try:
        text = p.read_text(encoding="latin-1", errors="ignore")
    except OSError:
        continue
    index["mmp"][str(p)] = {
        "libraries": [m.group(1).strip() for m in patterns["library"].finditer(text)],
        "capabilities": [m.group(1).strip() for m in patterns["capability"].finditer(text)],
    }

Path("symbian_sdk_index.json").write_text(
    json.dumps(index, indent=2, ensure_ascii=False), encoding="utf-8"
)
print("headers:", len(index["headers"]), "mmp:", len(index["mmp"]))
```

Convert the JSON findings back into reviewed Markdown before giving them to an agent.
