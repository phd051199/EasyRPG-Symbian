# Core reference maps and troubleshooting

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-09-reference-api-class-index-md"></a>
## Broad API Class Index

- Original corpus path: `09_reference/api_class_index.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-api-class-index-md`

### Broad API Class Index

Use this as a broad retrieval index. **Verify every non-core row in the installed FP2 SDK**, especially rows marked `verify`.

| Class/API | Area | Typical header | Typical library | Role |
|---|---|---|---|---|
| CBase | Core | e32base.h | euser.lib | Heap base class |
| CleanupStack | Core | e32base.h | euser.lib | Leave-safe cleanup |
| CActive | Core | e32base.h | euser.lib | Async request wrapper |
| CActiveScheduler | Core | e32base.h | euser.lib | Async dispatcher |
| CPeriodic | Core | e32base.h | euser.lib | Periodic callback timer |
| CTimer | Core | e32base.h | euser.lib | Timer active object base |
| RTimer | Core | e32std.h | euser.lib | Timer handle |
| RProcess | Core | e32std.h | euser.lib | Process handle |
| RThread | Core | e32std.h | euser.lib | Thread handle |
| RMutex | Core | e32std.h | euser.lib | Mutex |
| RSemaphore | Core | e32std.h | euser.lib | Semaphore |
| RChunk | Core | e32std.h | euser.lib | Shared memory chunk |
| RProperty | Core | e32property.h | euser.lib | Publish & Subscribe |
| TTime | Core | e32std.h | euser.lib | Time value |
| TLex | Core | e32std.h | euser.lib | Text parser |
| RArray | Core | e32base.h | euser.lib | Value array |
| RPointerArray | Core | e32base.h | euser.lib | Pointer array |
| RBuf | Core | e32base.h | euser.lib | Resizable descriptor |
| RFs | Filesystem | f32file.h | efsrv.lib | File server session |
| RFile | Filesystem | f32file.h | efsrv.lib | File handle |
| TEntry | Filesystem | f32file.h | efsrv.lib | File metadata |
| CDir | Filesystem | f32file.h | efsrv.lib | Directory entries |
| CFileMan | Filesystem | f32file.h | efsrv.lib | File management helper |
| RFileReadStream | Storage | s32file.h | estor.lib | Read stream |
| RFileWriteStream | Storage | s32file.h | estor.lib | Write stream |
| CBufFlat | Storage | s32mem.h | estor.lib | Flat memory buffer |
| RDbNamedDatabase | DBMS | d32dbms.h | edbms.lib | Named database |
| RDbView | DBMS | d32dbms.h | edbms.lib | Query view |
| RDbTable | DBMS | d32dbms.h | edbms.lib | Table access |
| CRepository | Settings | centralrepository.h | centralrepository.lib | Central Repository |
| CAknApplication | Avkon | aknapp.h | avkon.lib | Application |
| CAknDocument | Avkon | akndoc.h | avkon.lib | Document |
| CAknAppUi | Avkon | aknappui.h | avkon.lib | App UI |
| CAknViewAppUi | Avkon | aknviewappui.h | avkon.lib | View-based App UI |
| CAknView | Avkon | aknview.h | avkon.lib | View |
| CCoeControl | UI | coecntrl.h | cone.lib | Control base |
| CEikAppUi | UI | eikappui.h | eikcore.lib | Eikon AppUi |
| CEikStatusPane | UI | eikspane.h | eikcore.lib | Status pane |
| CEikMenuPane | UI | eikmenup.h | eikcore.lib | Menu pane |
| CEikEdwin | UI | eikedwin.h | eikcoctl.lib | Text editor |
| CAknMessageQueryDialog | UI | aknmessagequerydialog.h | avkon.lib | Message query |
| CAknTextQueryDialog | UI | aknquerydialog.h | avkon.lib | Text query |
| CAknInformationNote | UI | aknnotewrappers.h | avkon.lib | Information note |
| CAknErrorNote | UI | aknnotewrappers.h | avkon.lib | Error note |
| CAknSingleStyleListBox | UI | aknlists.h | avkon.lib | List box |
| CWindowGc | Graphics | w32std.h | ws32.lib | Window graphics context |
| RWsSession | Graphics | w32std.h | ws32.lib | Window server session |
| CWsScreenDevice | Graphics | w32std.h | ws32.lib | Screen device |
| RWindow | Graphics | w32std.h | ws32.lib | Window handle |
| CFbsBitmap | Graphics | fbs.h | fbscli.lib | Bitmap |
| CFbsBitmapDevice | Graphics | fbs.h | fbscli.lib | Bitmap drawing device |
| CFbsBitGc | Graphics | fbs.h | fbscli.lib | Bitmap graphics context |
| AknIconUtils | Graphics | akniconutils.h | aknicon.lib | Icon utility |
| RSocketServ | Networking | es_sock.h | esock.lib | Socket server |
| RSocket | Networking | es_sock.h | esock.lib | Socket |
| RHostResolver | Networking | es_sock.h | esock.lib | Name resolver |
| RConnection | Networking | es_sock.h | esock.lib | Bearer connection |
| TInetAddr | Networking | in_sock.h | insock.lib | IP address |
| RHTTPSession | HTTP | http.h | http.lib | HTTP session |
| RHTTPTransaction | HTTP | http.h | http.lib | HTTP transaction |
| RHTTPHeaders | HTTP | http.h | http.lib | HTTP headers |
| MHTTPTransactionCallback | HTTP | mhttptransactioncallback.h | http.lib | HTTP callback |
| MHTTPDataSupplier | HTTP | mhttpdatasupplier.h | http.lib | Request body supplier |
| RCommServ | Serial | c32comm.h | c32.lib | Comm server |
| RComm | Serial | c32comm.h | c32.lib | Serial port |
| TBTDevAddr | Bluetooth | bt_sock.h | bluetooth.lib | Bluetooth address |
| CBluetoothSocket | Bluetooth | bluetooth.h | bluetooth.lib | Bluetooth socket helper; verify |
| CMdaAudioPlayerUtility | Audio | mdaaudiosampleplayer.h | mediaclientaudio.lib | Audio player |
| CMdaAudioRecorderUtility | Audio | mdaaudiosampleeditor.h | mediaclientaudio.lib | Audio recorder/editor |
| CVideoPlayerUtility | Video | videoplayer.h | mediaclientvideo.lib | Video player |
| CCamera | Camera | ecam.h | ecam.lib | Camera |
| CImageDecoder | Images | imageconversion.h | imageconversion.lib | Image decoder |
| CImageEncoder | Images | imageconversion.h | imageconversion.lib | Image encoder |
| CFrameInfo | Images | imageconversion.h | imageconversion.lib | Frame metadata |
| CMsvSession | Messaging | msvapi.h | msgs.lib | Message server session |
| CMsvEntry | Messaging | msvapi.h | msgs.lib | Message entry context |
| CMsvOperation | Messaging | msvapi.h | msgs.lib | Async message operation |
| CContactDatabase | Contacts | cntdb.h | cntmodel.lib | Contacts database |
| CContactItem | Contacts | cntitem.h | cntmodel.lib | Contact item |
| CContactCard | Contacts | cntitem.h | cntmodel.lib | Contact card |
| CContactItemField | Contacts | cntfield.h | cntmodel.lib | Contact field |
| CCalSession | Calendar | calsession.h | calinterimapi.lib | Calendar session |
| CCalEntryView | Calendar | calentryview.h | calinterimapi.lib | Entry view |
| CCalInstanceView | Calendar | calinstanceview.h | calinterimapi.lib | Instance view |
| CCalEntry | Calendar | calentry.h | calinterimapi.lib | Calendar entry |
| CTelephony | Telephony | etel3rdparty.h | etel3rdparty.lib | High-level telephony |
| REComSession | ECom | ecom/ecom.h | ecom.lib | Plug-in session |
| CImplementationInformation | ECom | ecom/implementationinformation.h | ecom.lib | Plug-in metadata |
| HAL | HAL | hal.h | hal.lib | Hardware abstraction values |
| FeatureManager | Features | featmgr.h | featmgr.lib | Feature detection; verify |
| CDocumentHandler | Application services | documenthandler.h | commonengine.lib or SDK-specific | Open/send documents; verify |
| RFbsSession | Graphics | fbs.h | fbscli.lib | Bitmap server session |
| RDesReadStream | Storage | s32mem.h | estor.lib | Descriptor read stream |
| RDesWriteStream | Storage | s32mem.h | estor.lib | Descriptor write stream |
| RFileLogger | Logging | flogger.h | flogger.lib | File logger |
| RDebug | Debug | e32debug.h | euser.lib | Debug output |

#### Important

This index is intentionally class-level, not a verbatim member reference. Exact overloads, capabilities, leave codes, and availability belong to the FP2 Developer Library and installed headers.

---

<a id="src-09-reference-api-map-connectivity-md"></a>
## API Map: Connectivity

- Original corpus path: `09_reference/api_map_connectivity.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-api-map-connectivity-md`

### API Map: Connectivity

| API | Header | Library | Capability | Purpose |
| --- | --- | --- | --- | --- |
| RSocketServ | es_sock.h | esock.lib | NetworkServices for normal network use | Socket server session |
| RSocket | es_sock.h | esock.lib, insock.lib | NetworkServices / LocalServices by protocol | Socket subsession |
| RHostResolver | es_sock.h | esock.lib, insock.lib | NetworkServices | DNS/name resolution |
| RConnection | es_sock.h | esock.lib | NetworkServices | Bearer/IAP connection |
| RHTTPSession | http.h | http.lib | NetworkServices | HTTP session |
| RHTTPTransaction | http.h | http.lib | NetworkServices | HTTP request/response |
| Bluetooth socket types | bt_sock.h | bluetooth.lib, esock.lib | LocalServices | Bluetooth addressing/protocol |
| RCommServ / RComm | c32comm.h or SDK equivalent | c32.lib | Device/policy-specific | Serial communications |

The HTTP and Bluetooth stacks use additional string tables, filters, protocol modules, or service discovery APIs. Copy exact setup from an FP2 example.

---

<a id="src-09-reference-api-map-core-md"></a>
## API Map: Core Runtime and System

- Original corpus path: `09_reference/api_map_core.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-api-map-core-md`

### API Map: Core Runtime and System

Use this as a lookup accelerator, then verify exact declarations in the installed FP2 headers.

| API | Header | Library | Capability | Purpose |
| --- | --- | --- | --- | --- |
| CBase / new(ELeave) | e32base.h | euser.lib | None | Heap base and leave-aware allocation |
| CleanupStack | e32base.h | euser.lib | None | Leave-safe cleanup |
| CActive / CActiveScheduler | e32base.h | euser.lib | None | Async dispatch |
| RTimer | e32std.h / e32base.h | euser.lib | None | Async timers |
| RProcess / RThread | e32std.h | euser.lib | Varies | Process/thread handles |
| RProperty | e32property.h | euser.lib | Policy-specific | Publish & Subscribe |
| RFs / RFile | f32file.h | efsrv.lib | Path-specific | Filesystem |
| RFileReadStream / RFileWriteStream | s32file.h | estor.lib, efsrv.lib | Path-specific | Serialization |
| RDbNamedDatabase / RDbView | d32dbms.h | edbms.lib | Path/data-specific | Classic DBMS |
| CRepository | centralrepository.h | centralrepository.lib | Key policy-specific | Persistent settings |

#### Ownership reminders

- `C*`: delete unless ownership transferred.
- `R*`: `Close()`.
- Async provider: cancel before closing its handle.
- Borrowed descriptor/reference: do not delete.

---

<a id="src-09-reference-api-map-multimedia-pim-md"></a>
## API Map: Multimedia and PIM

- Original corpus path: `09_reference/api_map_multimedia_pim.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-api-map-multimedia-pim-md`

### API Map: Multimedia and PIM

| API | Header | Library | Capability | Purpose |
| --- | --- | --- | --- | --- |
| CMdaAudioPlayerUtility | mdaaudiosampleplayer.h | mediaclientaudio.lib | Usually none for playback | Audio playback |
| CVideoPlayerUtility | videoplayer.h | mediaclientvideo.lib | Varies | Video playback |
| CCamera | ecam.h | ecam.lib | UserEnvironment (verify) | Camera |
| CImageDecoder / CImageEncoder | imageconversion.h | imageconversion.lib | File/path-specific | Image codecs |
| CContactDatabase | cntdb.h | cntmodel.lib | ReadUserData/WriteUserData | Contacts |
| CCalSession | calsession.h | calinterimapi.lib | ReadUserData/WriteUserData | Calendar |
| CMsvSession | msvapi.h | msgs.lib | Operation-specific | Message server |
| CTelephony | etel3rdparty.h | etel3rdparty.lib | Method-specific | Telephony |
| LBS position APIs | lbs*.h | lbs.lib | Location | Positioning |
| Sensor Framework | sensrv*.h | sensrvclient.lib | Sensor/policy-specific | Sensors; plug-in |

Rows marked variable must be checked at method level and against installed plug-ins/firmware.

---

<a id="src-09-reference-api-map-ui-md"></a>
## API Map: Avkon and Graphics

- Original corpus path: `09_reference/api_map_ui.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-api-map-ui-md`

### API Map: Avkon and Graphics

| API | Header | Library | Purpose |
| --- | --- | --- | --- |
| CAknApplication | aknapp.h | avkon.lib, apparc.lib | Application object |
| CAknDocument | akndoc.h | avkon.lib | Document object |
| CAknAppUi | aknappui.h | avkon.lib, eikcore.lib | UI controller |
| CCoeControl | coecntrl.h | cone.lib | Custom control |
| CWindowGc | w32std.h / gdi.h | ws32.lib, gdi.lib or UI libs | Window drawing |
| CAknMessageQueryDialog | aknmessagequerydialog.h | avkon.lib | Message query |
| CAknInformationNote | aknnotewrappers.h | avkon.lib | Information note |
| CEikStatusPane | eikspane.h | eikcore.lib | Status pane |
| CFbsBitmap | fbs.h | fbscli.lib | Bitmap handle |
| AknIconUtils | akniconutils.h | aknicon.lib | Icon loading/scaling; verify SDK |

Library transitive linkage varies. If a symbol is unresolved, inspect the class reference and SDK example instead of adding random UI libraries.

---

<a id="src-09-reference-api-verification-workflow-md"></a>
## API Verification Workflow for Agents

- Original corpus path: `09_reference/api_verification_workflow.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-api-verification-workflow-md`

### API Verification Workflow for Agents

#### Goal

Confirm an API exists on S60 3rd FP2 before generating implementation code.

#### Step 1: class/header search

```bat
findstr /s /i /n "class CVideoPlayerUtility" %EPOCROOT%epoc32\include\*.h
findstr /s /i /n "SetVideoRectL" %EPOCROOT%epoc32\include\*.h
```

#### Step 2: inspect declaration

Record:

- Header path.
- Exact method signature and overloads.
- Observer interface.
- Async cancellation method.
- Capability comments if present.

#### Step 3: find SDK example

Search examples for the class and copy the minimum setup/lifecycle, not the entire application.

#### Step 4: find library

- Check developer library class page.
- Check example `.mmp`.
- If unresolved, search import-library symbols with available SDK tools.

#### Step 5: verify phone runtime

Check whether the API is base ROM, redistributable plug-in, or device-specific.

#### Required answer format

```text
Availability: base SDK / plug-in / not found
Header:
Library:
Capability:
Async model:
Cancellation:
Ownership:
FP2-specific caveat:
```

---

<a id="src-09-reference-compatibility-fp2-md"></a>
## FP2 Compatibility Checklist

- Original corpus path: `09_reference/compatibility_fp2.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-compatibility-fp2-md`

### FP2 Compatibility Checklist

#### Before importing code

- Does the source explicitly target S60 3rd FP2 or OS 9.3?
- Are all include files present in `%EPOCROOT%epoc32\include`?
- Are all `.lib` files present for WINSCW and GCCE?
- Are UI resource constants defined in FP2 `.rh` files?
- Does the code assume touch input?
- Does it use Symbian^3 display/window/video APIs?
- Does it require Open C/C++, API Plug-in Pack, Eseries, sensor, or OpenGL plug-ins?

#### Typical downgrade substitutions

| Later-code pattern | FP2 action |
|---|---|
| Touch event-only UI | implement keypad/focus commands |
| New layout enums | use FP2 layout/resource constants |
| Later video rectangle methods | use FP2 player constructor/display-window API |
| Qt Mobility sensor/location | use native FP2 plug-in APIs or remove feature |
| Modern TLS expectations | use compatibility proxy/backend |
| C++11/STL-heavy code | rewrite to supported compiler/runtime subset |

#### Build both targets early

A source file that builds only under WINSCW can still fail under GCCE. Build phone target after each subsystem milestone.

---

<a id="src-09-reference-error-codes-panics-md"></a>
## Error Codes and Panic Triage

- Original corpus path: `09_reference/error_codes_panics.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-error-codes-panics-md`

### Error Codes and Panic Triage

#### Common system errors

| Code name | Meaning/action |
|---|---|
| `KErrNone` | success |
| `KErrNotFound` | missing file/resource/service/IAP |
| `KErrGeneral` | generic failure; gather more context |
| `KErrCancel` | canceled request/user action |
| `KErrNoMemory` | allocation failed; reduce memory/use cleanup |
| `KErrNotSupported` | API/format/operation unsupported |
| `KErrArgument` | invalid parameter |
| `KErrBadHandle` | unopened/closed/invalid handle |
| `KErrInUse` | resource already owned or request outstanding |
| `KErrAlreadyExists` | duplicate file/object/server/property |
| `KErrPermissionDenied` | capability/policy/data-caging failure |
| `KErrTimedOut` | operation timeout |
| `KErrDisconnected` | link/bearer disconnected |
| `KErrCorrupt` | invalid file/data/database |

#### Panic triage

1. Record category and number.
2. Find category in headers/source/docs.
3. Inspect last descriptor/array/handle operation.
4. Confirm Active Object state and cancellation.
5. Check ownership and object lifetime.
6. Reproduce in UDEB with debug output/breakpoints.

#### `USER` and `KERN-EXEC`

These broad categories often require the panic number and call stack. `KERN-EXEC 3` frequently indicates invalid memory access; it is not a codec error code.

---

<a id="src-09-reference-full-reference-rag-import-md"></a>
## Importing the Official FP2 Developer Library into RAG

- Original corpus path: `09_reference/full_reference_rag_import.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-full-reference-rag-import-md`

### Importing the Official FP2 Developer Library into RAG

#### Why keep it separate

This corpus provides original practical guides. The archived **S60 3rd Ed FP2 C++ Developer's Library v2.0** contains the exhaustive member-level reference and should be ingested separately under its own license/usage constraints.

#### Suggested local workflow

1. Obtain `S60v3_FP2_Developers_Library_v2_0.zip` from the archived SDK collection.
2. Scan the archive and extract it in an isolated workspace.
3. Prefer its HTML/XML source rather than rendering pages to PDF.
4. Convert one API page at a time to Markdown while preserving:
   - class name
   - namespace
   - header
   - library
   - capability
   - since/deprecation/platform notes
   - method signature
   - parameters
   - return/leave/panic behavior
5. Store the original relative URL/path as metadata.
6. Deduplicate navigation/header/footer text.
7. Chunk per class and per method group.

#### Recommended metadata

```yaml
platform: S60 3rd Edition FP2
os: Symbian OS 9.3
reference_version: 2.0.1.0.0
kind: official-api-reference
class: RSocket
header: es_sock.h
library: esock.lib
```

#### Retrieval strategy

Retrieve this practical corpus first for architecture and patterns, then official class/method chunks for exact signatures. Never use a Symbian^3 chunk when an FP2 chunk exists.

#### Legal note

Do not redistribute a converted full official library unless its license permits it. Keep a locally generated RAG index private when license status is unclear.

---

<a id="src-09-reference-glossary-md"></a>
## Glossary

- Original corpus path: `09_reference/glossary.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-glossary-md`

### Glossary

| Term | Meaning |
|---|---|
| ABLD | classic Symbian build command wrapper |
| Active Object | cooperative async request wrapper dispatched by Active Scheduler |
| Avkon | Nokia S60 UI application framework |
| Capability | platform-security permission |
| Cleanup Stack | leave-safe resource cleanup stack |
| Descriptor | bounded text/binary buffer abstraction |
| ECom | plug-in discovery/instantiation framework |
| EPOCROOT | active SDK root path variable |
| GCCE | GNU compiler toolchain for ARM phone binaries |
| IAP | Internet Access Point configuration |
| Leave | Symbian exception-like error transfer |
| MMF | Multimedia Framework |
| MMP | project definition file |
| MTM | Messaging Type Module |
| RCOMP | resource compiler |
| SID | Secure ID |
| SIS/SISX | Symbian installation package / signed package |
| TRK | on-device debug agent |
| UID | unique identifier |
| WINSCW | emulator compiler/target |

---

<a id="src-09-reference-mmp-library-recipe-md"></a>
## MMP Library Recipes

- Original corpus path: `09_reference/mmp_library_recipe.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-mmp-library-recipe-md`

### MMP Library Recipes

#### Basic Avkon application

```text
LIBRARY euser.lib apparc.lib cone.lib eikcore.lib avkon.lib
```

#### Files and streams

```text
LIBRARY efsrv.lib estor.lib
```

#### TCP/IP sockets

```text
LIBRARY esock.lib insock.lib
CAPABILITY NetworkServices
```

#### HTTP

```text
LIBRARY http.lib esock.lib
CAPABILITY NetworkServices
```

Additional HTTP/string/filter libraries may be needed by the exact sample/API use.

#### Audio

```text
LIBRARY mediaclientaudio.lib
```

#### Video

```text
LIBRARY mediaclientvideo.lib ws32.lib
```

Exact display dependencies vary.

#### Images

```text
LIBRARY imageconversion.lib fbscli.lib
```

#### Contacts

```text
LIBRARY cntmodel.lib
CAPABILITY ReadUserData WriteUserData
```

#### Calendar

```text
LIBRARY calinterimapi.lib
CAPABILITY ReadUserData WriteUserData
```

#### Central Repository

```text
LIBRARY centralrepository.lib
```

#### Rule

Only add a library when a referenced imported symbol requires it. Header inclusion and linking solve different problems.

---

<a id="src-09-reference-source-catalog-md"></a>
## Source Catalog and Verification Links

- Original corpus path: `09_reference/source_catalog.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-source-catalog-md`

### Source Catalog and Verification Links

#### Archived tool and documentation sources

- S60/Nokia SDK and tool archive: https://archive.org/details/nokia_sdks_n_dev_tools
- Carbide.c++ archive index: https://mrrosset.github.io/Symbian-Archive/Dep%26IDE.html
- S60 3rd FP2 Developer's Library archive index: https://mrrosset.github.io/Symbian-Archive/SDKs-Docs.html
- API Plug-in Packs: https://mrrosset.github.io/Symbian-Archive/SDKs-Plugins.html
- Online S60 3rd Edition C++ Developer's Library mirror: https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/
- Symbian C++ introduction mirror: https://katastrophos.net/symbian-dev/GUID-C3206E31-251C-4AFC-90C2-04B38CCF8E18.html
- Windows setup notes: https://gist.github.com/artem78/cb2b9650af186844f7b5654964676284
- Programming for Symbian OS book: https://girtel.upct.es/~eegea/pub/symbian_book.pdf
- Symbian application-development slides: https://www.medien.ifi.lmu.de/diamd05/slides/symbian.pdf
- Alternative Linux/macOS toolchain project: https://www.martin.st/symbian/

#### Relevant archive filenames

The Internet Archive collection lists files including:

- `S60-3.2-SDK-f.inc3.2130.zip`
- `S60_3rd_FP2_v2.xml.zip` / related device metadata
- `S60v3_FP2_Developers_Library_v2_0.zip`
- `S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip`
- `carbide_cpp_v2_7_en.zip`
- `s60_open_c_cpp_plug_in_v1_7_en.zip` or equivalent Open C/C++ package names
- Eseries/Sensor/OpenGL ES FP2 plug-ins

#### Source use policy for agents

- Use these sources to verify exact API signatures and platform notes.
- Do not mix later-edition method signatures into FP2 code.
- Treat archive binaries as untrusted downloads: scan them and use a disposable VM snapshot.
- This corpus is an original practical summary, not a substitute for member-by-member reference documentation.

---

<a id="src-09-reference-troubleshooting-md"></a>
## Troubleshooting Build, Resource, Link, Run, and Install Errors

- Original corpus path: `09_reference/troubleshooting.md`
- Consolidated reference: `10-core-reference-troubleshooting.md#src-09-reference-troubleshooting-md`

### Troubleshooting Build, Resource, Link, Run, and Install Errors

#### `Program file does not exist`

The launch step cannot find the EXE because compilation/resource/linking failed. Scroll to the first error, not the final launch message.

#### `RCOMP failed - deleting output files`

Find the first resource error. Common causes:

- Unknown enum/resource identifier.
- Missing `.rh`/`.hrh` include.
- Syntax error before the reported line.
- Symbol from another S60 edition.
- Wrong generated `.rsg` name.

#### Header cannot be opened

- Wrong SDK active.
- Missing API plug-in.
- Header name from later SDK.
- Incorrect `USERINCLUDE`/`SYSTEMINCLUDE`.
- Plug-in installed before SDK or into another SDK root.

#### Undefined identifier

Search installed headers. If absent, the API is unavailable or named differently. Do not create a dummy declaration unless implementing your own function/class.

#### Linker undefined symbol

The header was found, but the import library is missing or wrong. Find the class reference or SDK example and add the correct `.lib`.

#### `CPeriodic::Start` mismatch

Use the interval types declared by the FP2 header. Explicitly construct `TTimeIntervalMicroSeconds32` or the required type.

#### SIS will not install

- UID conflict.
- Certificate expired/not yet valid relative to device date.
- Capability not granted.
- Platform dependency mismatch.
- Downgrade/update policy.
- Missing/corrupt package file.

#### Runs then panics

- Null object from failed/unfinished async initialization.
- Descriptor overflow.
- Array index out of range.
- Double delete/close.
- Callback arriving after owner destruction.
- UI control left on control stack while deleted.
