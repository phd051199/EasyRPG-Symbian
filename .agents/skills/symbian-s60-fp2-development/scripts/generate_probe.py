#!/usr/bin/env python3
"""Generate a minimal Symbian compile/link probe project."""
from __future__ import annotations
import argparse
from pathlib import Path
import re


def safe_ident(value: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_]", "_", value)
    if not value or value[0].isdigit():
        value = "Probe_" + value
    return value


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--name", default="api-probe")
    ap.add_argument("--header", required=True)
    ap.add_argument("--symbol", required=True)
    ap.add_argument("--library", action="append", default=[])
    ap.add_argument("--capability", default="None")
    ap.add_argument("--out", default="./symbian-api-probe")
    args = ap.parse_args()

    root = Path(args.out)
    group = root / "group"
    src = root / "src"
    group.mkdir(parents=True, exist_ok=True)
    src.mkdir(parents=True, exist_ok=True)

    target = safe_ident(args.name)
    libs = ["euser.lib"] + [x for x in args.library if x.lower() != "euser.lib"]
    lib_lines = "\n".join(f"LIBRARY         {x}" for x in libs)

    (group / "bld.inf").write_text("PRJ_PLATFORMS\nDEFAULT\n\nPRJ_MMPFILES\nprobe.mmp\n", encoding="utf-8")
    (group / "probe.mmp").write_text(f"""TARGET          {target}.exe
TARGETTYPE      exe
UID             0x100039CE 0xE00000F0
SECUREID        0xE00000F0
CAPABILITY      {args.capability}
SOURCEPATH      ..\\src
SOURCE          probe.cpp
SYSTEMINCLUDE   \\epoc32\\include
{lib_lines}
""", encoding="utf-8")
    (src / "probe.cpp").write_text(f"""#include <e32base.h>
#include <{args.header}>

// Confirm the exact declaration in the installed FP2 header before editing this probe.
static void ReferenceSymbolL()
{{
    // Target symbol: {args.symbol}
    // Add the smallest valid declaration, construction, or call here.
}}

GLDEF_C TInt E32Main()
{{
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    TRAPD(err, ReferenceSymbolL());
    delete cleanup;
    __UHEAP_MARKEND;
    return err;
}}
""", encoding="utf-8")
    (root / "README.md").write_text(f"""# {args.name}

Target symbol: `{args.symbol}`  
Header: `{args.header}`  
Libraries: `{", ".join(libs)}`  
Capabilities: `{args.capability}`

```bat
cd group
bldmake bldfiles
abld build winscw udeb
abld build gcce urel
```

Edit `src/probe.cpp` to reference one exact FP2 declaration. A successful header compile does not prove device runtime support.
""", encoding="utf-8")
    print(f"Generated probe at {root.resolve()}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
