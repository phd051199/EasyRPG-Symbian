---
name: symbian-s60-fp2-development
description: Design, implement, debug, port, package, and review native Symbian C++ applications for Symbian OS 9.3 / S60 3rd Edition Feature Pack 2 using Carbide.c++ 2.7, WINSCW, GCCE, Avkon, S60 APIs, API Plug-ins, SIS/SISX, and Nokia devices such as E72. Use for architecture decisions, API discovery, avoiding reimplementation of platform services, class/header/library/capability lookup, build errors, resource compiler failures, panics, signing, networking, multimedia, PIM, telephony, location, and compatibility checks against newer S60/Symbian code.
compatibility: Agent Skills clients. Python 3 is recommended for bundled search and validation scripts. Compiling requires a Windows environment with S60 3rd FP2 SDK and Carbide.c++ 2.7 or compatible command-line tools.
metadata:
  author: community-symbian-development
  version: "1.0.0"
  target-platform: s60-3rd-edition-fp2
  symbian-os: "9.3"
---

# Symbian S60 FP2 Development Skill

Use this skill as the operating procedure for native S60 3rd Edition FP2 work. Do not rely on modern Symbian, S60 5th Edition, Qt, Android, or generic C++ assumptions unless the task explicitly involves porting from them.

## Core rules

1. Treat the installed S60 3rd FP2 SDK as the final compile-time authority.
2. A Symbian OS 9.3 class page does **not** prove that the class is shipped in the S60 FP2 SDK.
3. Distinguish every dependency as one of: base SDK, API Plug-in, Open C/C++, Eseries/device extension, internal/private API, or unavailable.
4. Prefer an existing platform service over a custom subsystem when it satisfies the requirement. Search the reuse cookbook before designing new infrastructure.
5. Never invent a class, enum, resource ID, overload, import library, capability, or callback signature.
6. Verify availability in this order: S60 API guide → installed header → import library or matching `.mmp` example → capability/signing → device runtime support.
7. Explain ownership, leave behavior, Cleanup Stack use, asynchronous completion, cancellation, and teardown for every non-trivial API.
8. Build and test both `WINSCW UDEB` and `GCCE UREL`; emulator success is not device proof.
9. Preserve source links in technical answers so uncertain details can be checked quickly.
10. When evidence is incomplete, label the uncertainty and provide an exact local verification command instead of guessing.

## Start every task with this decision flow

### 1. Classify the request

Identify the dominant task:

- architecture or feature design;
- API discovery or “is there already an API?”;
- implementation or code generation;
- porting from another S60 edition;
- compile/resource/link error;
- runtime leave/panic;
- packaging/signing/deployment;
- performance, memory, or asynchronous lifecycle review.

### 2. Search before loading references

Run:

```bash
python3 scripts/search_reference.py "<symbols and concepts>" --top 12
```

Use exact identifiers when available, for example:

```bash
python3 scripts/search_reference.py "CVideoPlayerUtility SetVideoRectL mediaclientvideo.lib" --top 15
python3 scripts/search_reference.py "RCmManager destination connection method" --top 12
python3 scripts/search_reference.py "RCOMP Cannot convert EDataLayoutDataEd" --top 12
```

Read [references/00-catalog.md](references/00-catalog.md) only to navigate. Do not load every reference bundle into context.

### 3. Check whether the platform already solves the problem

For architecture and complex features, load:

- [references/60-reuse-cookbook-index.md](references/60-reuse-cookbook-index.md)
- the matching `61`–`63` reuse bundle;
- the matching API catalog bundle `21`–`24`;
- the relevant domain bundle `04`–`09`.

Write an API decision record using [assets/api-decision-record.md](assets/api-decision-record.md) when the choice affects architecture.

### 4. Produce a compatibility verdict

Before code, state:

```text
Target: S60 3rd Edition FP2 / Symbian OS 9.3
Status: base SDK | plug-in required | device extension | deprecated | unavailable | local verification required
Evidence: API guide / header / class page / SDK example / compile probe
```

For a symbol from newer code, search the exact symbol in the bundled references and then in the installed SDK. Never replace a missing identifier with a declaration copied from a newer SDK.

### 5. Resolve build metadata

For every API used, resolve:

- exact `#include`;
- exact `LIBRARY` entries in `.mmp`;
- `CAPABILITY` requirements per operation;
- resource headers and `.rss` dependencies;
- UID/SID/signing implications;
- whether an API Plug-in or device extension must be installed.

Use [references/50-header-reference-index.md](references/50-header-reference-index.md), the matching header bundle, and [references/40-class-guide-index.md](references/40-class-guide-index.md).

If a library or capability is not confirmed, say so and run or suggest:

```bat
findstr /s /n /i /c:"ClassOrMethod" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"ClassOrMethod" C:\S60\devices\*.mmp
```

### 6. Design ownership and asynchronous flow

For each object or handle, document:

- creator and owner;
- `NewL/NewLC/delete`, `Open/Close`, or borrowed ownership;
- every function that may leave;
- descriptor and observer lifetime;
- request state machine;
- completion callback;
- provider-specific cancel method;
- teardown order.

Load [references/03-symbian-cpp-runtime.md](references/03-symbian-cpp-runtime.md) for descriptors, leaves, two-phase construction, Active Objects, IPC, threads, and ECom.

### 7. Generate code in FP2 style

Code must:

- use descriptors instead of unsafe C-string assumptions;
- use two-phase construction where allocation plus leaving initialization is required;
- use Cleanup Stack correctly across leaves;
- close `R` handles in reverse dependency order;
- cancel outstanding asynchronous operations before destruction;
- avoid Standard C++ exceptions for classic Symbian framework boundaries;
- keep `M*` observers and async buffers alive through completion;
- include only headers actually needed;
- include matching `.mmp`, `.rss`, `.pkg`, capability, and signing changes.

Use templates from `assets/` only after adapting exact signatures to the installed FP2 headers.

### 8. Verify with a minimal probe

When API availability or linking is uncertain, generate a probe:

```bash
python3 scripts/generate_probe.py \
  --name video-probe \
  --header videoplayer.h \
  --symbol CVideoPlayerUtility \
  --library mediaclientvideo.lib \
  --capability None \
  --out ./probe
```

Then build:

```bat
cd probe\group
bldmake bldfiles
abld build winscw udeb
abld build gcce urel
```

Header compile, linker success, install/sign success, and real-device behavior are separate verification stages.

## Error-triage workflow

### Compiler or missing header

1. Identify the first error, not the final missing EXE message.
2. Search the exact symbol/header.
3. Determine edition and plug-in provenance.
4. Check `USERINCLUDE`, `SYSTEMINCLUDE`, and active SDK.
5. Build a one-symbol compile probe.

### Resource compiler (`RCOMP`)

1. Inspect the first `.rss` line reported.
2. Verify every `.rh`, `.hrh`, and generated `.rsg` include.
3. Check whether the enum/resource belongs to S60 5th or Symbian^3.
4. Do not convert an unresolved identifier to a numeric guess.

### Linker unresolved symbol

1. Confirm the declaration exists in the active FP2 header.
2. Find a matching SDK example or class-page “Link against” field.
3. Add the exact import library.
4. Clean and rebuild both targets.
5. If the `.lib` is absent, identify the required plug-in or replace the API.

### Runtime panic or leave

1. Record panic category/code or leave code.
2. Check construction state, ownership, array indices, descriptor bounds, and active requests.
3. Verify control-stack and view/container lifecycles.
4. Add emulator breakpoints/logging and test negative paths.
5. Use [references/10-core-reference-troubleshooting.md](references/10-core-reference-troubleshooting.md).

## Domain routing

Load only what matches the task:

| Task | References |
|---|---|
| SDK, Carbide, WINSCW, GCCE, TRK | `02-environment-build-toolchain.md` |
| Descriptors, Cleanup Stack, Active Objects, IPC, ECom | `03-symbian-cpp-runtime.md` |
| AppUi, View Framework, controls, resources, keys | `04-avkon-application-ui.md` |
| Files, DBMS, CenRep, P&S, XML, timers | `05-system-services.md` |
| Sockets, HTTP, IAP, WLAN, Bluetooth | `06-connectivity-networking.md` |
| Contacts, calendar, messaging, location, telephony | `07-pim-location-telephony.md` |
| Audio, video, camera, image codecs, graphics | `08-multimedia-graphics.md` |
| Capabilities, UID/SID, SIS/SISX, signing | `09-security-packaging-deployment.md` |
| Exact S60 API group | `20`–`24` API catalog bundles |
| Exact public header | `50`–`58` header bundles |
| Common class lifecycle/build metadata | `40`–`43` class bundles |
| API Plug-in inventory | `30`–`32` plug-in bundles |
| Avoiding reimplementation | `60`–`63` reuse bundles |
| Source/archive URLs | `15-source-links.md` |

## Required answer structure for substantial tasks

Use this structure unless the user asks for a narrower response:

1. **Compatibility verdict**
2. **Existing platform APIs to reuse**
3. **Recommended architecture**
4. **Headers, libraries, capabilities, and plug-ins**
5. **Ownership and async lifecycle**
6. **Implementation/code**
7. **Build/resource/package changes**
8. **Validation matrix**
9. **Risks and fallback for unavailable APIs**
10. **Referenced documentation links**

For small error fixes, keep the answer compact but still include the verified SDK/edition conclusion.

## Local SDK tools

- `scripts/search_reference.py`: search the bundled handbook by symbol/concept.
- `scripts/scan_sdk.py`: index installed headers, classes, methods, `.mmp` libraries, and capabilities.
- `scripts/generate_probe.py`: create a minimal WINSCW/GCCE compile-link probe.
- `scripts/validate_skill.py`: validate skill structure, limits, links, and frontmatter.

## Safety against false certainty

Do not claim a method is available merely because a web page or OS reference contains it. Do not claim a capability applies to an entire header when it is operation-specific. Do not call a private/internal header a public compatibility contract. When device firmware or codec/provider support can vary, require an actual Nokia FP2 device test.
