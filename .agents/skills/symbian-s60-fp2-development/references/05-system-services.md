# System services and storage APIs

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-04-system-apis-content-access-framework-md"></a>
## Content Access Framework and Protected Content

- Original corpus path: `04_system_apis/content_access_framework.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-content-access-framework-md`

### Content Access Framework and Protected Content

#### Purpose

The Content Access Framework (CAF) abstracts access to plain and rights-managed content. Applications dealing with media files may encounter CAF objects even when they do not implement DRM themselves.

#### Concepts

- Data/content objects opened through CAF.
- Intent such as play, view, execute, or print.
- Rights evaluation and consumption.
- Attributes such as MIME type and protection state.

#### Multimedia interaction

A media controller may fail because rights are missing, expired, or incompatible. Treat this separately from codec failure.

#### Security

Do not attempt to bypass rights checks or copy protected content through private APIs. Use documented intent and error handling.

#### Verification

Search FP2 headers for `ContentAccess`, `CData`, `TVirtualPathPtr`, and CAF examples. Exact include/library names must come from the installed SDK.

---

<a id="src-04-system-apis-dbms-md"></a>
## DBMS and Structured Local Data

- Original corpus path: `04_system_apis/dbms.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-dbms-md`

### DBMS and Structured Local Data

#### Classic Symbian DBMS

Core classes include:

- `RDbNamedDatabase`
- `RDbView`
- `RDbTable`
- `CDbColSet`

Typical header/library:

```text
#include <d32dbms.h>
LIBRARY edbms.lib
```

#### Lifecycle

1. Connect `RFs` if file-backed operations need it.
2. Create/open database.
3. Execute schema SQL.
4. Prepare a view/table.
5. Insert/update/delete.
6. Close views before database.

#### Transactions

Use transactions for groups of updates. Commit on success, rollback on failure. This is important on flash storage and for consistency.

#### Views

A view represents a query result and has its own navigation/update lifecycle. Validate whether the view is read-only or updatable.

#### Alternatives

For small settings, use Central Repository. For simple records, streams/files may be easier. SQLite availability depends on installed components and should not be assumed in a base FP2 native project.

---

<a id="src-04-system-apis-feature-manager-hal-md"></a>
## Feature Manager, HAL, and Device Information

- Original corpus path: `04_system_apis/feature_manager_hal.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-feature-manager-hal-md`

### Feature Manager, HAL, and Device Information

#### Feature detection

Use platform feature APIs where available instead of assuming hardware from model name. Feature Manager APIs can test feature UIDs, but initialization and library names must match the SDK.

Potential pattern:

```cpp
FeatureManager::InitializeLibL();
TBool supported = FeatureManager::FeatureSupported(KFeatureIdSomething);
FeatureManager::UnInitializeLib();
```

Verify header/library names and cleanup rules in FP2.

#### HAL

`HAL::Get()` exposes selected hardware/system attributes.

```cpp
#include <hal.h>
#include <hal_data.h>
TInt value = 0;
TInt err = HAL::Get(HALData::EDisplayXPixels, value);
```

Typical library: `hal.lib`.

#### Device information

IMEI, subscriber, network, and firmware information may use telephony or device-management APIs and can require protected capabilities. Do not scrape private files.

#### Rule

Feature detection answers "supported by platform/device"; successful resource acquisition answers "available right now". Check both.

---

<a id="src-04-system-apis-filesystem-streams-md"></a>
## Filesystem, Files, and Streams

- Original corpus path: `04_system_apis/filesystem_streams.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-filesystem-streams-md`

### Filesystem, Files, and Streams

#### Main APIs

| Class | Header | Library |
|---|---|---|
| `RFs`, `RFile`, `TEntry`, `CDir` | `f32file.h` | `efsrv.lib` |
| `RFileReadStream`, `RFileWriteStream` | `s32file.h` | `estor.lib` plus `efsrv.lib` |

#### Session and file

```cpp
RFs fs;
User::LeaveIfError(fs.Connect());
CleanupClosePushL(fs);

RFile file;
User::LeaveIfError(file.Replace(fs, _L("C:\\Data\\MyApp\\x.txt"), EFileWrite));
CleanupClosePushL(file);
_LIT8(KData, "hello\r\n");
User::LeaveIfError(file.Write(KData));

CleanupStack::PopAndDestroy(&file);
CleanupStack::PopAndDestroy(&fs);
```

#### Private directory

An application can use its private directory based on `SecureId`. Prefer private storage for internal state. Cross-process access depends on platform security.

#### Directory creation

Use `MkDirAll()` for nested paths. Handle `KErrAlreadyExists` where appropriate.

#### Streams

```cpp
RFileWriteStream out;
out.Replace(fs, path, EFileWrite);
CleanupClosePushL(out);
out.WriteInt32L(value);
out.CommitL();
CleanupStack::PopAndDestroy(&out);
```

#### Atomic persistence

Write a temporary file, flush/commit, then rename over the destination. A direct rewrite can corrupt state on power loss.

#### Path rules

Use backslashes and drive letters. Validate removable-drive availability and do not assume `E:` exists.

---

<a id="src-04-system-apis-logging-debugging-md"></a>
## Logging, Panics, Leaves, and Debugging

- Original corpus path: `04_system_apis/logging_debugging.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-logging-debugging-md`

### Logging, Panics, Leaves, and Debugging

#### Debug output

```cpp
#include <e32debug.h>
RDebug::Print(_L("state=%d err=%d"), state, err);
```

#### File logging

Use `RFileLogger` where available, or a small `RFs/RFile` logger. Avoid excessive flash writes.

#### Panic creation

```cpp
_LIT(KPanicCategory, "MyApp");
User::Panic(KPanicCategory, EPanicBadState);
```

Use panics for programmer errors, not ordinary network failure.

#### Capturing failures

Record:

- Build target and variant.
- Panic category and number.
- Leave/error code.
- Last successful state transition.
- Device model/firmware.
- Capability/signing state.

#### Debug heap checks

Symbian debug builds support heap-mark patterns/macros in suitable contexts. Use SDK-provided examples and avoid enabling intrusive checks in production.

#### `KERN-EXEC 3`

Commonly points to invalid memory access, bad descriptor, null pointer, or array index. Inspect ownership/lifetime before blaming the emulator.

---

<a id="src-04-system-apis-properties-cenrep-md"></a>
## Publish & Subscribe and Central Repository

- Original corpus path: `04_system_apis/properties_cenrep.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-properties-cenrep-md`

### Publish & Subscribe and Central Repository

#### Publish & Subscribe (`RProperty`)

Use for lightweight system-wide values/events identified by category UID and key.

```cpp
#include <e32property.h>
TInt value = 0;
TInt err = RProperty::Get(KMyCategory, KMyKey, value);
```

Defining a property includes read/write security policies. Subscribing is asynchronous and commonly wrapped in an Active Object.

#### Central Repository (`CRepository`)

Use for persistent settings identified by repository UID and key.

```cpp
#include <centralrepository.h>
CRepository* repo = CRepository::NewLC(KCRUidMySettings);
User::LeaveIfError(repo->Set(KKeyVolume, 5));
CleanupStack::PopAndDestroy(repo);
```

Typical library:

```text
centralrepository.lib
```

#### Choosing

| Need | API |
|---|---|
| Persistent configuration | Central Repository |
| Fast observable state/event | Publish & Subscribe |
| Structured application data | File/stream or DBMS |

#### Security

Actual access is governed by repository/property policy, not merely by linking the library. Protected keys can require capabilities such as `ReadDeviceData` or `WriteDeviceData`.

#### Change notifications

Both systems support notification patterns. Re-subscribe after each completion where the API is one-shot.

---

<a id="src-04-system-apis-time-timers-md"></a>
## Time, Dates, and Timers

- Original corpus path: `04_system_apis/time_timers.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-time-timers-md`

### Time, Dates, and Timers

#### Time values

- `TTime`: absolute time.
- `TDateTime`: decomposed fields.
- `TTimeIntervalMicroSeconds`, `TTimeIntervalSeconds`, etc.: intervals.

```cpp
TTime now;
now.HomeTime();
```

Use UTC for persisted/network timestamps when possible and convert for display.

#### Timers

- `RTimer`: raw asynchronous timer.
- `CTimer`: Active Object timer base.
- `CPeriodic`: repeating callback timer.

#### `CPeriodic`

```cpp
iPeriodic = CPeriodic::NewL(CActive::EPriorityLow);
iPeriodic->Start(delay, interval, TCallBack(Tick, this));
```

Check the exact interval overloads in the FP2 headers. `TTimeIntervalMicroSeconds32` versus 64-bit interval mismatches are a common compiler error.

#### Clock changes

Wall-clock changes can affect absolute timers. For elapsed duration, use suitable interval/counter mechanisms rather than assuming system time is monotonic.

---

<a id="src-04-system-apis-xml-parsing-md"></a>
## XML Parsing and Serialization

- Original corpus path: `04_system_apis/xml_parsing.md`
- Consolidated reference: `05-system-services.md#src-04-system-apis-xml-parsing-md`

### XML Parsing and Serialization

#### Options

S60/Symbian SDKs can expose XML Framework/SAX-style APIs, while Open C/C++ or third-party libraries may add alternatives. Exact headers and libraries vary by SDK and plug-in.

Common concepts include:

- Parser object selected by MIME/type or implementation UID.
- Content-handler callbacks for document, element, text, and error events.
- 8-bit descriptors for encoded XML data.
- Namespace/attribute containers.

#### SAX design

A SAX parser streams callbacks and is memory-efficient. The content handler must copy any data that must outlive the callback.

#### Encoding

Honor the XML declaration and input encoding. Do not assume an 8-bit descriptor is UTF-8 unless the producer guarantees it.

#### Robustness

- Limit document size and nesting if parsing untrusted input.
- Handle partial network/file chunks according to parser API.
- Do not update UI controls directly from a worker-thread parser callback.

#### Verification

Search the installed SDK for `Xml::`, `MContentHandler`, `CParser`, `xmleng`, and XML examples. Later Symbian XML Engine APIs may not match the FP2 base SDK.
