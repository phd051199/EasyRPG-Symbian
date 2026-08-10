# Environment, Carbide.c++ 2.7, SDK, and build toolchain

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-01-environment-build-system-bld-inf-mmp-md"></a>
## Symbian Build System: bld.inf, MMP, Resources

- Original corpus path: `01_environment/build_system_bld_inf_mmp.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-build-system-bld-inf-mmp-md`

### Symbian Build System: bld.inf, MMP, Resources

#### `bld.inf`

Minimal form:

```text
PRJ_PLATFORMS
DEFAULT

PRJ_MMPFILES
MyApp.mmp
```

Optional export sections can copy headers, data, or other build inputs.

#### `.mmp` anatomy

```text
TARGET          MyApp.exe
TARGETTYPE      exe
UID             0x100039CE 0xE0001234
SECUREID        0xE0001234
VENDORID        0
CAPABILITY      NetworkServices

SOURCEPATH      ..\src
SOURCE          MyAppApplication.cpp
SOURCE          MyAppSymbianDocument.cpp
SOURCE          MyAppSymbianAppUi.cpp
SOURCE          MyAppView.cpp

USERINCLUDE     ..\inc
SYSTEMINCLUDE   \epoc32\include

START RESOURCE  ..\data\MyApp.rss
HEADER
TARGETPATH      resource\apps
END

START RESOURCE  ..\data\MyApp_reg.rss
TARGETPATH      \private\10003a3f\import\apps
END

LIBRARY         euser.lib
LIBRARY         apparc.lib
LIBRARY         cone.lib
LIBRARY         eikcore.lib
LIBRARY         avkon.lib
```

#### Key directives

- `TARGET`: output name.
- `TARGETTYPE`: usually `exe`, sometimes `dll` or other platform-specific target.
- `UID`: UID2 followed by UID3 for an application executable.
- `SECUREID`: process identity, normally equal to the application's UID3.
- `CAPABILITY`: platform-security grants requested by the binary.
- `SOURCEPATH` and `SOURCE`: translation units.
- `USERINCLUDE`: project headers.
- `SYSTEMINCLUDE`: SDK headers.
- `LIBRARY`: import libraries required at link time.
- `START RESOURCE`: resource compilation blocks.

#### Diagnosis by failure stage

| Failure | Likely file |
|---|---|
| RCOMP / RSS error | `.rss`, `.rh`, generated `.rsg` |
| Header not found | `.mmp` include paths, missing plug-in |
| Undefined identifier | wrong header/API edition |
| Undefined symbol at link | missing `.lib` or incompatible binary |
| SIS missing file | `.pkg` path or build configuration |

#### Rebuild after `.mmp` changes

When adding source, libraries, capability, or resources:

```bat
abld reallyclean
bldmake clean
bldmake bldfiles
abld build winscw udeb
```

Incremental builds sometimes keep stale generated metadata.

---

<a id="src-01-environment-carbide-2-7-setup-md"></a>
## Carbide.c++ 2.7 Setup

- Original corpus path: `01_environment/carbide_2_7_setup.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-carbide-2-7-setup-md`

### Carbide.c++ 2.7 Setup

#### Installation order

1. Install the S60 3rd FP2 SDK.
2. Install Carbide.c++ 2.7.
3. Start Carbide once and rescan SDKs.
4. Install API plug-ins only after the SDK is visible.
5. Reopen Carbide and rescan again.

#### Practical paths

Use short ASCII paths:

```text
C:\S60\devices\S60_3rd_FP2_SDK_v1.1\
C:\Symbian\Tools\Carbide.c++ v2.7\
C:\work\MyApp\
```

Avoid spaces in project paths when old Perl/build scripts behave unpredictably. Carbide itself can live in a path with spaces, but project and SDK paths are safer when simple.

#### SDK detection

In Carbide:

```text
Window -> Preferences -> Carbide.c++ -> SDK Preferences -> Rescan SDKs
```

At a configured SDK command prompt:

```bat
devices
devices -setdefault @<device-id>:com.nokia.s60
```

Do not copy the example device ID literally. Use the ID printed by `devices`.

#### Importing old projects

Use:

```text
File -> Import -> Symbian OS bld.inf file
```

Select `group\bld.inf`, then add:

- Emulator Debug (`WINSCW`, `UDEB`)
- Phone Release (`GCCE`, `UREL`)

#### Workspace hygiene

- Disable aggressive antivirus scanning of `epoc32` if builds become extremely slow, but keep the downloaded installers scanned.
- Keep one workspace per SDK family when possible.
- Do not let a S60 5th project silently bind to FP2 or vice versa.
- Refresh/reindex after installing plug-ins.

#### Carbide launch configurations

A build configuration creates binaries. A launch configuration runs or debugs them. A missing EXE means the build failed; changing the launch path does not fix compilation.

---

<a id="src-01-environment-device-gcce-trk-md"></a>
## GCCE Phone Builds and App TRK Debugging

- Original corpus path: `01_environment/device_gcce_trk.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-device-gcce-trk-md`

### GCCE Phone Builds and App TRK Debugging

#### Phone build

```bat
abld build gcce urel
```

Expected binary:

```text
%EPOCROOT%epoc32\release\gcce\urel\MyApp.exe
```

#### App TRK

Use the App TRK package matching S60 3rd FP2. Start TRK on the device, select the transport, then use a Carbide Symbian device launch configuration.

#### Device checklist

- Correct USB connection mode and Nokia connectivity driver.
- App TRK running and listening.
- Correct COM/USB transport selected.
- GCCE Phone Release configuration active.
- SIS/SISX signed with sufficient capability grants.
- Device date compatible with certificate validity.

#### Why a phone build can fail after emulator success

- ARM compiler rejects assumptions tolerated by WINSCW.
- Missing phone-side DLL or codec.
- Capability denial.
- Path under another application's private directory.
- Alignment, timing, or uninitialized-memory bug.
- API plug-in exists in the SDK but not in the target firmware.

#### Release versus debug

Most normal device packages use `GCCE UREL`. A device debug build may be available in specific setups, but source-level TRK workflows and SDK configuration vary. Do not hardcode UDEB paths into a release `.pkg`.

---

<a id="src-01-environment-emulator-winscw-md"></a>
## WINSCW Emulator Workflow

- Original corpus path: `01_environment/emulator_winscw.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-emulator-winscw-md`

### WINSCW Emulator Workflow

#### Purpose

WINSCW builds x86 binaries for the S60 emulator. They are not phone binaries.

#### Normal workflow

```bat
cd group
bldmake bldfiles
abld build winscw udeb
```

Expected executable:

```text
%EPOCROOT%epoc32\release\winscw\udeb\MyApp.exe
```

#### Emulator filesystem mapping

The emulator presents Symbian drives, but files are backed by SDK directories. ROM-like content is commonly staged below `epoc32\data\z`; writable drive content is under emulator-specific directories.

#### Emulator limitations

- Hardware codecs may differ from the phone.
- Camera, sensors, telephony, Bluetooth, and bearer selection may be absent or simulated.
- Timing and memory behavior differ from ARM hardware.
- Platform security may be more permissive in some emulator configurations.
- UI layout can differ from a specific device skin.

#### Debugging

Set breakpoints after a clean build. For leaves, inspect the leave code and call stack. For panics, capture the panic category and number.

Useful debug output:

```cpp
#include <e32debug.h>
RDebug::Print(_L("Value=%d"), value);
```

Avoid assuming emulator success proves device success. It proves only that one x86 configuration survived.

---

<a id="src-01-environment-open-c-cpp-md"></a>
## Open C/C++ Plug-in and Standard C/C++ Compatibility

- Original corpus path: `01_environment/open_c_cpp.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-open-c-cpp-md`

### Open C/C++ Plug-in and Standard C/C++ Compatibility

#### Purpose

Open C/C++ adds a broader C/POSIX runtime and selected C++ library support to Symbian. It can ease porting code that expects `stdio`, file descriptors, sockets, pthread-like APIs, or standard library components.

#### Trade-offs

- Target devices may need Open C runtime SIS packages.
- Native Symbian APIs remain preferable for Avkon UI, platform services, descriptors, and Active Objects.
- Mixing POSIX blocking calls into the UI thread can freeze the application.
- Compiler/runtime support is not modern C++11.

#### Packaging

Include required runtime dependencies according to the plug-in documentation. A build that links successfully can still fail on a phone missing runtime DLLs.

#### Interop

Convert carefully between:

- `TDesC` Unicode paths and narrow C strings.
- Symbian error codes/leaves and `errno`.
- Active Object async architecture and blocking POSIX calls.

#### Recommendation

Use Open C/C++ for ported libraries in a contained layer. Keep application/UI/service integration native.

---

<a id="src-01-environment-sdk-layout-and-tools-md"></a>
## SDK Layout and Toolchain Commands

- Original corpus path: `01_environment/sdk_layout_and_tools.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-sdk-layout-and-tools-md`

### SDK Layout and Toolchain Commands

#### Important directories

| Path | Purpose |
|---|---|
| `epoc32\include` | Public and plug-in headers |
| `epoc32\release\winscw\udeb` | Emulator debug binaries |
| `epoc32\release\gcce\urel` | GCCE phone binaries |
| `epoc32\data\z` | Emulator ROM/data staging tree |
| `epoc32\tools` | Build, resource, SIS, and utility tools |
| `examples` | SDK examples; names vary by SDK release |

#### Core commands

```bat
bldmake bldfiles
abld build winscw udeb
abld build gcce urel
abld clean winscw udeb
abld reallyclean
makesis App.pkg App_unsigned.sis
signsis -s App_unsigned.sis App.sisx cert.cer key.key
```

#### Build phases

1. `bldmake bldfiles` generates platform build metadata from `bld.inf` and `.mmp`.
2. `abld export` copies exported resources/data.
3. `abld resource` runs resource/bitmap stages.
4. `abld target` compiles and links.
5. `abld build` runs the normal combined path.

#### Environment variables

`EPOCROOT` traditionally begins and ends with a backslash, for example:

```text
\S60\devices\S60_3rd_FP2_SDK_v1.1\
```

A malformed `EPOCROOT`, broken `PATH`, or incorrect active device can produce errors far away from the actual cause.

#### Useful diagnostics

```bat
where perl
where abld
where makesis
where signsis
echo %EPOCROOT%
devices
```

#### Search before guessing

```bat
findstr /s /i /n "class RHTTPSession" %EPOCROOT%epoc32\include\*.h
findstr /s /i /n "SetVideoRectL" %EPOCROOT%epoc32\include\*.h
```

For old SDKs, direct header search is often faster and more reliable than modern web search.

---

<a id="src-01-environment-testing-and-profiling-md"></a>
## Testing, Profiling, and Diagnostics

- Original corpus path: `01_environment/testing_and_profiling.md`
- Consolidated reference: `02-environment-build-toolchain.md#src-01-environment-testing-and-profiling-md`

### Testing, Profiling, and Diagnostics

#### Test layers

1. Pure C++ logic tests where possible.
2. WINSCW emulator tests for UI and basic service integration.
3. GCCE package install/run tests.
4. Real-device tests for hardware, codecs, network, power, and capabilities.

#### Useful tools

- Carbide debugger and breakpoints.
- App TRK for on-device debugging.
- `RDebug::Print` and file logging.
- S60 Performance Investigator/profiler packages where compatible.
- Nokia Energy Profiler and optional external API plug-in.
- Emulator debug heap checks.

#### Performance priorities

On E72-era hardware, optimize:

- Allocations in callbacks/draw loops.
- Bitmap size and decode resolution.
- Network wakeups/retries.
- Filesystem writes.
- Active Object callback duration.
- Repeated descriptor conversions.

#### Reproducibility

Record SDK build, plug-ins, device firmware, signing certificate capabilities, and exact build configuration with every bug report.
