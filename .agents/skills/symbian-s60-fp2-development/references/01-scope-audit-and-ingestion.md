# Scope, evidence model, audit, and ingestion

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-00-audit-availability-model-md"></a>
## API availability model

- Original corpus path: `00_AUDIT/AVAILABILITY_MODEL.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-availability-model-md`

### API availability model

Use these labels in answers and generated code:

| Label | Meaning |
|---|---|
| `verified_quick_reference` | API group is listed in the S60 release Quick Reference. |
| `verified_header` | Header name is confirmed by the S60 Quick Reference or class page. |
| `verified_class_page` | Class page confirms at least header/library/capability or signature. |
| `verified_installed_sdk` | Header/import library was found in the user's installed SDK. |
| `plugin_v1_2_listed` | API appears in the preserved FP2 plug-in v1.2 publication. |
| `plugin_v1_3_exists` | Archive confirms a v1.3 pack exists, but its contents were not inspected here. |
| `device_extension` | API comes from Eseries/device/sensor/etc. extension. |
| `not_in_fp2_sdk` | Documentation explicitly says it is not part of S60 3rd FP2 SDK. |
| `local_sdk_check` | Metadata is plausible but must be resolved on the user's SDK. |

#### Decision procedure

```text
request -> API group -> header exists? -> import library exists? -> capability?
       -> target device supports feature? -> compile probe -> runtime probe
```

Never “repair” an unavailable API by inventing a declaration. Port to a supported API,
install the documented plug-in, or gate the feature.

---

<a id="src-00-audit-confidence-and-hallucination-policy-md"></a>
## CONFIDENCE AND HALLUCINATION POLICY

- Original corpus path: `00_AUDIT/CONFIDENCE_AND_HALLUCINATION_POLICY.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-confidence-and-hallucination-policy-md`

### Confidence and anti-hallucination policy

For every code answer, distinguish:

- exact SDK fact;
- documented S60 API-group metadata;
- operation-dependent capability;
- likely import library needing local confirmation;
- device-specific behavior;
- unverified recollection.

An agent must never:

- invent an Avkon enum because a newer SDK has a similar name;
- assume a method exists on FP2 because it exists on S60 5th/Symbian^3;
- declare a missing imported function manually to silence the compiler;
- add capabilities blindly;
- claim an API Plug-in v1.3 member solely because it was listed in v1.2;
- use an OS reference page as proof of SDK inclusion.

---

<a id="src-00-audit-coverage-report-md"></a>
## API handbook coverage report

- Original corpus path: `00_AUDIT/COVERAGE_REPORT.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-coverage-report-md`

### Coverage report

#### Generated coverage

- API-group manuals: **121**
- Unique public-header guides: **566**
- Common class guides retained/enriched: **75**
- Problem-oriented reuse recipes: **64**
- FP2 API Plug-in catalogue entries retained: **48**

#### What “complete usage guide” means here

Every S60 Quick Reference API group now includes:

- decision: when to use it and what not to reinvent;
- purpose/version/deprecation;
- all public headers listed by Quick Reference;
- header role analysis;
- architecture, ownership and async/cancel rules;
- step-by-step usage flow;
- build library/capability resolution;
- skeleton/example;
- failure modes and device test matrix;
- direct API-index and global official reference links;
- offline Developer Library/API Plug-in links.

#### Honest boundary

This is not a verbatim mirror of every copyrighted method page. Method signatures and
per-member capability lines are edition-sensitive, so the final authority remains:

1. the linked FP2 class/member page;
2. the installed FP2 SDK header;
3. matching SDK example `.mmp`;
4. GCCE linker/device behavior.

The included local corpus generators can convert an offline Developer Library and SDK
headers into method-level Markdown for the exact installation.

---

<a id="src-00-audit-design-without-reinventing-md"></a>
## DESIGN WITHOUT REINVENTING

- Original corpus path: `00_AUDIT/DESIGN_WITHOUT_REINVENTING.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-design-without-reinventing-md`

### Design complex Symbian apps without reinventing platform services

#### Layering

```text
UI (Avkon views/controls)
  -> application services (your orchestration/state machines)
    -> Symbian adapters (HTTP, sockets, media, contacts, location, settings)
      -> platform servers/frameworks
```

#### Rules

- One adapter owns each R-session/utility and its Active Objects.
- UI never directly owns long-running request buffers.
- Convert callbacks into explicit states: idle/opening/ready/running/cancelling/error.
- Centralize capability and MMP metadata beside each adapter.
- Feature-detect optional services and provide degraded behavior.
- Prefer platform selectors/editors/viewers over duplicated UI.
- Use transactions/query APIs instead of ad-hoc file/database scans.
- Treat emulator as debug aid, not hardware/service compatibility proof.

Start every feature search in `21_REUSE_COOKBOOK/000_REUSE_MAP.md`.

---

<a id="src-00-audit-os93-is-not-s60-fp2-md"></a>
## Symbian OS 9.3 reference is not the S60 FP2 SDK inventory

- Original corpus path: `00_AUDIT/OS93_IS_NOT_S60_FP2.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-os93-is-not-s60-fp2-md`

### Symbian OS 9.3 reference is not the S60 FP2 SDK inventory

A class may have a complete Symbian OS 9.3 reference page and still be absent from the
S60 3rd Edition FP2 SDK.

Documented counterexample:

- class: `RSmsSocketReadStream`
- header: `smsustrm.h`
- library: `smsu.lib`
- the reference page explicitly states that the item is **not part of the S60 3rd
  Edition SDK for Symbian OS, Feature Pack 2**.

Therefore an agent must not use “I found a v9.3 class page” as availability proof.

#### Correct response pattern

```text
The class exists in the OS reference, but its FP2 SDK availability is not established.
I will check epoc32/include, epoc32/release/... import libraries, and matching plug-ins.
```

---

<a id="src-00-audit-source-audit-md"></a>
## Source audit

- Original corpus path: `00_AUDIT/SOURCE_AUDIT.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-source-audit-md`

### Source audit

#### Primary documentation layers

1. **S60 3rd Edition C++ Developer's Library**
   - Supports S60 3rd Edition, FP1 and FP2.
   - Contains introductory, technology-area and API-level material.
   - URL: https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html

2. **S60 Quick Reference**
   - Alphabetical list of APIs included in that release.
   - Carries first-published/deprecated platform levels and public header names.
   - URL: https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html

3. **Symbian OS v9.3 C++ class reference**
   - Useful for exact class/member pages.
   - Must not be treated as proof that the class ships in S60 3rd FP2 SDK.

4. **Installed S60 3rd FP2 SDK**
   - Final compile-time authority for headers, import libraries, resource headers and tools.

5. **API Plug-in / Eseries / device-specific extension**
   - Adds APIs that are not in the base SDK.
   - Install only after the matching SDK.

6. **Open Symbian source repositories**
   - Useful for implementation and `.mmp` cross-checks.
   - Example: https://github.com/SymbianSource/oss.FCL.sf.mw.classicui

#### Archive facts used by this corpus

- The archive catalogue lists **S60 3rd Ed SDK FP2 API Plug-In Pack v1.3 (2010)**.
- A preserved v1.2 publication lists 48 APIs. This corpus does not silently assume
  that the v1.3 file set is byte-for-byte identical.
- The agent must inspect the installed v1.3 package before marking a plug-in API as
  `verified_installed_v1_3`.

#### Copyright treatment

This corpus is an original, reorganized guide and metadata index. It does not reproduce
the complete Nokia/Symbian manuals verbatim. API names, header names, versions, class
signatures and build metadata are factual compatibility information.

---

<a id="src-00-audit-validation-report-md"></a>
## VALIDATION REPORT

- Original corpus path: `00_AUDIT/VALIDATION_REPORT.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-audit-validation-report-md`

### Validation report

Generated: 2026-06-25T01:41:50+00:00

#### Corpus

- Markdown files: **993** after adding this report
- API manuals: **121**
- Header guides: **567**
- Reuse recipes: **65**
- Local Markdown links checked outside code fences: **3123**
- External HTTP(S) links recorded outside code fences: **5535**
- Missing real local links: **0**
- API manuals missing required sections: **0**
- Exact duplicate Markdown bodies before this report: **0**

#### ZIP policy

The release ZIP is rebuilt after this report and contains Markdown files only.

#### Notes

- External links are recorded and grouped by API, but mirrors can disappear; the offline
  package links and local conversion workflow are retained as fallback.
- Direct API-index URLs follow the exact target naming used by the Quick Reference. A
  global Quick Reference fallback is included in every API manual.
- Library/capability fields that lack class/member evidence are deliberately marked for
  local SDK verification rather than guessed.

---

<a id="src-00-scope-and-compatibility-md"></a>
## Scope and Compatibility: OS 9.3 / S60 3rd FP2

- Original corpus path: `00_scope_and_compatibility.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-00-scope-and-compatibility-md`

### Scope and Compatibility: OS 9.3 / S60 3rd FP2

#### Platform identity

For this corpus:

- **Symbian OS 9.3** means the base OS generation.
- **S60 3rd Edition Feature Pack 2** is the Nokia UI/platform layer commonly called **S60 3.2**.
- Nokia E72 and several late keypad-era S60 devices belong to this target family, although firmware and device plug-ins can still differ.

#### Compatibility zones

| Source material | Trust level for FP2 |
|---|---|
| Installed FP2 headers, libraries, examples | Authoritative for the installed SDK |
| S60 3rd FP2 Developer's Library | High |
| S60 3rd MR / FP1 docs | Useful, but check additions/removals |
| S60 5th Edition docs | Conceptual only; touch/UI APIs may differ |
| Symbian^3/Belle docs | Often incompatible at source and resource level |
| Qt for Symbian docs | Separate framework; do not mix with Avkon by default |

#### Base SDK versus plug-ins

A class may be documented for S60 but absent from the base SDK. Common add-ons include:

- S60 3rd FP2 API Plug-in Pack v1.3.
- Open C/C++ plug-in.
- Eseries SDK plug-in.
- Sensor API plug-in.
- OpenGL ES plug-in.
- Device-specific plug-ins.

Always confirm by searching under:

```text
%EPOCROOT%epoc32\include
%EPOCROOT%epoc32\release\armv5\lib
%EPOCROOT%epoc32\release\gcce\urel
```

#### Binary and source compatibility

Symbian preserves many binary contracts, but source code can still fail because:

- Headers changed location.
- An API was partner-only or plug-in-only.
- A method overload was added later.
- Capability policy changed.
- Resource symbols differ by UI edition.
- A device omits a codec or hardware feature.

#### Safe porting strategy

1. Build the smallest baseline application first.
2. Add one subsystem at a time.
3. For each unresolved symbol, identify header and library from the installed SDK.
4. For each resource error, verify the `.rh`/`.rsg` include and platform version.
5. Test emulator behavior and real-device behavior separately.
6. Treat codec, camera, telephony, sensor, and network bearer behavior as device-dependent.

---

<a id="src-agent-ingestion-md"></a>
## Agent Ingestion and Chunking Guide

- Original corpus path: `AGENT_INGESTION.md`
- Consolidated reference: `01-scope-audit-and-ingestion.md#src-agent-ingestion-md`

### Agent Ingestion and Chunking Guide

#### Recommended ingestion

- Parse YAML front matter as metadata.
- Chunk by H2/H3 sections, not fixed character count alone.
- Preserve code fences with the paragraph introducing them.
- Attach the file path and `applies_to` metadata to every chunk.
- Give higher retrieval priority to `09_reference/api_map_*.md` for lookup, then retrieve the subsystem guide for lifecycle/context.

#### Suggested chunk metadata

```json
{
  "platform": "Symbian OS 9.3",
  "ui": "S60 3rd Edition FP2",
  "ide": "Carbide.c++ 2.7",
  "source_file": "05_connectivity/http_client.md",
  "confidence": "medium",
  "requires_sdk_verification": true
}
```

#### Prompt rule

Tell the coding agent:

> Only generate APIs confirmed for S60 3rd Edition FP2. For every non-core API, return header, library, capability, async lifecycle, and whether a plug-in is required. When uncertain, instruct the user to search the installed SDK headers instead of inventing a symbol.

#### Retrieval combinations

- Compiler error -> `troubleshooting` + corresponding `api_map` + subsystem guide.
- New feature -> subsystem guide + `capabilities_matrix` + `mmp_library_recipe`.
- Porting -> `compatibility_fp2` + subsystem guide.
- Packaging -> UID guide + SIS/signing guide + capabilities.
