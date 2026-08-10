# Common class guides 051–075

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-12-class-guides-051-cmsvsession-md"></a>
## CMsvSession

- Original corpus path: `12_CLASS_GUIDES/051_cmsvsession.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-051-cmsvsession-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: msvapi.h
- `library`: msgs.lib
- `capability`: ReadUserData, WriteUserData for message data

### `CMsvSession`

#### Role

Message Server session.

#### Build metadata

- Header: `msvapi.h`
- Import library: `msgs.lib`
- Capability: `ReadUserData, WriteUserData for message data`
- Confidence: `verified-common`

```cpp
#include <msvapi.h>
```

```text
LIBRARY         msgs.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

OpenSyncL/OpenAsyncL; observe server events and keep entry contexts valid.

#### Compile-time verification

1. Search `msvapi.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `msgs.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CMsvSession`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CMsvSessionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CMsvSession`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CMsvSession%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-052-cmsventry-md"></a>
## CMsvEntry

- Original corpus path: `12_CLASS_GUIDES/052_cmsventry.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-052-cmsventry-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: msvapi.h
- `library`: msgs.lib
- `capability`: ReadUserData, WriteUserData

### `CMsvEntry`

#### Role

Message Server entry context.

#### Build metadata

- Header: `msvapi.h`
- Import library: `msgs.lib`
- Capability: `ReadUserData, WriteUserData`
- Confidence: `verified-common`

```cpp
#include <msvapi.h>
```

```text
LIBRARY         msgs.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

SetEntryL() changes context; children and store operations can leave.

#### Compile-time verification

1. Search `msvapi.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `msgs.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CMsvEntry`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CMsvEntryClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CMsvEntry`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CMsvEntry%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-053-ccontactdatabase-md"></a>
## CContactDatabase

- Original corpus path: `12_CLASS_GUIDES/053_ccontactdatabase.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-053-ccontactdatabase-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: cntdb.h
- `library`: cntmodel.lib
- `capability`: ReadUserData, WriteUserData

### `CContactDatabase`

#### Role

Contacts database client.

#### Build metadata

- Header: `cntdb.h`
- Import library: `cntmodel.lib`
- Capability: `ReadUserData, WriteUserData`
- Confidence: `verified-common`

```cpp
#include <cntdb.h>
```

```text
LIBRARY         cntmodel.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

OpenL(), read/open contact items, commit changes, observe database locking.

#### Compile-time verification

1. Search `cntdb.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `cntmodel.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CContactDatabase`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CContactDatabaseClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CContactDatabase`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CContactDatabase%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-054-ccontactitem-md"></a>
## CContactItem

- Original corpus path: `12_CLASS_GUIDES/054_ccontactitem.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-054-ccontactitem-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: cntitem.h
- `library`: cntmodel.lib
- `capability`: ReadUserData, WriteUserData

### `CContactItem`

#### Role

Base contact record.

#### Build metadata

- Header: `cntitem.h`
- Import library: `cntmodel.lib`
- Capability: `ReadUserData, WriteUserData`
- Confidence: `verified-common`

```cpp
#include <cntitem.h>
```

```text
LIBRARY         cntmodel.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Field storage uses contact field types/content types; database owns IDs, caller owns loaded item.

#### Compile-time verification

1. Search `cntitem.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `cntmodel.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CContactItem`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CContactItemClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CContactItem`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CContactItem%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-055-ccalsession-md"></a>
## CCalSession

- Original corpus path: `12_CLASS_GUIDES/055_ccalsession.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-055-ccalsession-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: calsession.h
- `library`: calinterimapi.lib
- `capability`: ReadUserData, WriteUserData

### `CCalSession`

#### Role

Calendar Interim session.

#### Build metadata

- Header: `calsession.h`
- Import library: `calinterimapi.lib`
- Capability: `ReadUserData, WriteUserData`
- Confidence: `verified-common`

```cpp
#include <calsession.h>
```

```text
LIBRARY         calinterimapi.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect and open a calendar file before creating entry/instance views.

#### Compile-time verification

1. Search `calsession.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `calinterimapi.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CCalSession`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CCalSessionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CCalSession`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CCalSession%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-056-ccalentry-md"></a>
## CCalEntry

- Original corpus path: `12_CLASS_GUIDES/056_ccalentry.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-056-ccalentry-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: calentry.h
- `library`: calinterimapi.lib
- `capability`: ReadUserData, WriteUserData

### `CCalEntry`

#### Role

Calendar entry data object.

#### Build metadata

- Header: `calentry.h`
- Import library: `calinterimapi.lib`
- Capability: `ReadUserData, WriteUserData`
- Confidence: `verified-common`

```cpp
#include <calentry.h>
```

```text
LIBRARY         calinterimapi.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

UID, recurrence, time mode and exception semantics must remain consistent.

#### Compile-time verification

1. Search `calentry.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `calinterimapi.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CCalEntry`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CCalEntryClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CCalEntry`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CCalEntry%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-057-ctelephony-md"></a>
## CTelephony

- Original corpus path: `12_CLASS_GUIDES/057_ctelephony.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-057-ctelephony-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: etel3rdparty.h
- `library`: etel3rdparty.lib
- `capability`: operation-specific phone capabilities

### `CTelephony`

#### Role

Third-party telephony facade.

#### Build metadata

- Header: `etel3rdparty.h`
- Import library: `etel3rdparty.lib`
- Capability: `operation-specific phone capabilities`
- Confidence: `verified-common`

```cpp
#include <etel3rdparty.h>
```

```text
LIBRARY         etel3rdparty.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use asynchronous methods and check the exact capability on each operation in local docs.

#### Compile-time verification

1. Search `etel3rdparty.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `etel3rdparty.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CTelephony`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CTelephonyClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CTelephony`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CTelephony%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-058-rpositionserver-md"></a>
## RPositionServer

- Original corpus path: `12_CLASS_GUIDES/058_rpositionserver.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-058-rpositionserver-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: lbs.h
- `library`: lbs.lib
- `capability`: Location

### `RPositionServer`

#### Role

Location Server session.

#### Build metadata

- Header: `lbs.h`
- Import library: `lbs.lib`
- Capability: `Location`
- Confidence: `verified-quickref`

```cpp
#include <lbs.h>
```

```text
LIBRARY         lbs.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect, open RPositioner, set requestor and update options.

#### Compile-time verification

1. Search `lbs.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `lbs.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RPositionServer`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RPositionServerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RPositionServer`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RPositionServer%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-059-rpositioner-md"></a>
## RPositioner

- Original corpus path: `12_CLASS_GUIDES/059_rpositioner.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-059-rpositioner-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: lbs.h
- `library`: lbs.lib
- `capability`: Location

### `RPositioner`

#### Role

Position request sub-session.

#### Build metadata

- Header: `lbs.h`
- Import library: `lbs.lib`
- Capability: `Location`
- Confidence: `verified-quickref`

```cpp
#include <lbs.h>
```

```text
LIBRARY         lbs.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

NotifyPositionUpdate() is asynchronous and must be cancelled on teardown.

#### Compile-time verification

1. Search `lbs.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `lbs.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RPositioner`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RPositionerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RPositioner`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RPositioner%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-060-cposlandmarkdatabase-md"></a>
## CPosLandmarkDatabase

- Original corpus path: `12_CLASS_GUIDES/060_cposlandmarkdatabase.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-060-cposlandmarkdatabase-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: epos_cposlandmarkdatabase.h
- `library`: lbslandmarks.lib
- `capability`: Location / data-policy

### `CPosLandmarkDatabase`

#### Role

Landmark database access.

#### Build metadata

- Header: `epos_cposlandmarkdatabase.h`
- Import library: `lbslandmarks.lib`
- Capability: `Location / data-policy`
- Confidence: `verified-quickref`

```cpp
#include <epos_cposlandmarkdatabase.h>
```

```text
LIBRARY         lbslandmarks.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Many operations return CPosLmOperation for asynchronous execution.

#### Compile-time verification

1. Search `epos_cposlandmarkdatabase.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `lbslandmarks.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CPosLandmarkDatabase`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CPosLandmarkDatabaseClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CPosLandmarkDatabase`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CPosLandmarkDatabase%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-061-recomsession-md"></a>
## REComSession

- Original corpus path: `12_CLASS_GUIDES/061_recomsession.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-061-recomsession-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: ecom/ecom.h
- `library`: ecom.lib
- `capability`: None

### `REComSession`

#### Role

ECom implementation discovery and lifecycle.

#### Build metadata

- Header: `ecom/ecom.h`
- Import library: `ecom.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <ecom/ecom.h>
```

```text
LIBRARY         ecom.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use CreateImplementationL/ListImplementationsL and call FinalClose() at safe shutdown points.

#### Compile-time verification

1. Search `ecom/ecom.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `ecom.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `REComSession`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/REComSessionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `REComSession`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22REComSession%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-062-featuremanager-md"></a>
## FeatureManager

- Original corpus path: `12_CLASS_GUIDES/062_featuremanager.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-062-featuremanager-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: featmgr.h
- `library`: featmgr.lib
- `capability`: None

### `FeatureManager`

#### Role

Runtime feature flag query.

#### Build metadata

- Header: `featmgr.h`
- Import library: `featmgr.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <featmgr.h>
```

```text
LIBRARY         featmgr.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

InitializeLibL()/UnInitializeLib() must be balanced in non-framework use.

#### Compile-time verification

1. Search `featmgr.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `featmgr.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `FeatureManager`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/FeatureManagerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `FeatureManager`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22FeatureManager%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-063-cdocumenthandler-md"></a>
## CDocumentHandler

- Original corpus path: `12_CLASS_GUIDES/063_cdocumenthandler.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-063-cdocumenthandler-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: documenthandler.h
- `library`: commonui.lib
- `capability`: operation-specific

### `CDocumentHandler`

#### Role

Launches viewers/editors for content.

#### Build metadata

- Header: `documenthandler.h`
- Import library: `commonui.lib`
- Capability: `operation-specific`
- Confidence: `verified-quickref`

```cpp
#include <documenthandler.h>
```

```text
LIBRARY         commonui.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use MIME/type recognition and handle absence of a suitable handler.

#### Compile-time verification

1. Search `documenthandler.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `commonui.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CDocumentHandler`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CDocumentHandlerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CDocumentHandler`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CDocumentHandler%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-064-chwrmlight-md"></a>
## CHWRMLight

- Original corpus path: `12_CLASS_GUIDES/064_chwrmlight.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-064-chwrmlight-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: hwrmlight.h
- `library`: hwrmlightclient.lib
- `capability`: device-policy

### `CHWRMLight`

#### Role

HWRM light client.

#### Build metadata

- Header: `hwrmlight.h`
- Import library: `hwrmlightclient.lib`
- Capability: `device-policy`
- Confidence: `verified-quickref`

```cpp
#include <hwrmlight.h>
```

```text
LIBRARY         hwrmlightclient.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Targets and permissions differ by device; release control promptly.

#### Compile-time verification

1. Search `hwrmlight.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `hwrmlightclient.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CHWRMLight`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CHWRMLightClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CHWRMLight`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CHWRMLight%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-065-chwrmvibra-md"></a>
## CHWRMVibra

- Original corpus path: `12_CLASS_GUIDES/065_chwrmvibra.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-065-chwrmvibra-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: hwrmvibra.h
- `library`: hwrmvibraclient.lib
- `capability`: device-policy

### `CHWRMVibra`

#### Role

HWRM vibration client.

#### Build metadata

- Header: `hwrmvibra.h`
- Import library: `hwrmvibraclient.lib`
- Capability: `device-policy`
- Confidence: `verified-quickref`

```cpp
#include <hwrmvibra.h>
```

```text
LIBRARY         hwrmvibraclient.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Respect profile/settings and maximum duration; observe status where available.

#### Compile-time verification

1. Search `hwrmvibra.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `hwrmvibraclient.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CHWRMVibra`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CHWRMVibraClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CHWRMVibra`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CHWRMVibra%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-066-rconnectionmonitor-md"></a>
## RConnectionMonitor

- Original corpus path: `12_CLASS_GUIDES/066_rconnectionmonitor.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-066-rconnectionmonitor-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: rconnmon.h
- `library`: connmon.lib
- `capability`: NetworkServices

### `RConnectionMonitor`

#### Role

Connection Monitor client.

#### Build metadata

- Header: `rconnmon.h`
- Import library: `connmon.lib`
- Capability: `NetworkServices`
- Confidence: `verified-quickref`

```cpp
#include <rconnmon.h>
```

```text
LIBRARY         connmon.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect, query attributes and subscribe to events; event payload types vary.

#### Compile-time verification

1. Search `rconnmon.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `connmon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RConnectionMonitor`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RConnectionMonitorClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RConnectionMonitor`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RConnectionMonitor%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-067-cbluetoothsynchronouslink-md"></a>
## CBluetoothSynchronousLink

- Original corpus path: `12_CLASS_GUIDES/067_cbluetoothsynchronouslink.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-067-cbluetoothsynchronouslink-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: bt_sock.h
- `library`: bluetooth.lib
- `capability`: LocalServices

### `CBluetoothSynchronousLink`

#### Role

Bluetooth synchronous-link wrapper.

#### Build metadata

- Header: `bt_sock.h`
- Import library: `bluetooth.lib`
- Capability: `LocalServices`
- Confidence: `verified-class-page`

```cpp
#include <bt_sock.h>
```

```text
LIBRARY         bluetooth.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

This class page explicitly identifies header, library and LocalServices capability.

#### Compile-time verification

1. Search `bt_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `bluetooth.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CBluetoothSynchronousLink`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CBluetoothSynchronousLinkClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CBluetoothSynchronousLink`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CBluetoothSynchronousLink%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-068-rbtphysicallinkadapter-md"></a>
## RBTPhysicalLinkAdapter

- Original corpus path: `12_CLASS_GUIDES/068_rbtphysicallinkadapter.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-068-rbtphysicallinkadapter-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: bt_sock.h
- `library`: bluetooth.lib
- `capability`: LocalServices

### `RBTPhysicalLinkAdapter`

#### Role

Bluetooth physical-link control.

#### Build metadata

- Header: `bt_sock.h`
- Import library: `bluetooth.lib`
- Capability: `LocalServices`
- Confidence: `verified-common`

```cpp
#include <bt_sock.h>
```

```text
LIBRARY         bluetooth.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Open against socket server/connection context; operations are device-stack dependent.

#### Compile-time verification

1. Search `bt_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `bluetooth.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RBTPhysicalLinkAdapter`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RBTPhysicalLinkAdapterClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RBTPhysicalLinkAdapter`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RBTPhysicalLinkAdapter%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-069-csendui-md"></a>
## CSendUi

- Original corpus path: `12_CLASS_GUIDES/069_csendui.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-069-csendui-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: sendui.h
- `library`: sendui.lib
- `capability`: ReadUserData depending attachments

### `CSendUi`

#### Role

Builds Send via UI and invokes sending services.

#### Build metadata

- Header: `sendui.h`
- Import library: `sendui.lib`
- Capability: `ReadUserData depending attachments`
- Confidence: `verified-quickref`

```cpp
#include <sendui.h>
```

```text
LIBRARY         sendui.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Construct message data and let service capability filtering drive available transports.

#### Compile-time verification

1. Search `sendui.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `sendui.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CSendUi`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CSendUiClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CSendUi`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CSendUi%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-070-cmessagedata-md"></a>
## CMessageData

- Original corpus path: `12_CLASS_GUIDES/070_cmessagedata.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-070-cmessagedata-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: cmessagedata.h
- `library`: sendui.lib
- `capability`: ReadUserData depending attachments

### `CMessageData`

#### Role

Message body, subject, recipients and attachments for Send UI.

#### Build metadata

- Header: `cmessagedata.h`
- Import library: `sendui.lib`
- Capability: `ReadUserData depending attachments`
- Confidence: `verified-quickref`

```cpp
#include <cmessagedata.h>
```

```text
LIBRARY         sendui.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Attachment paths must remain valid while the editor/service consumes them.

#### Compile-time verification

1. Search `cmessagedata.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `sendui.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CMessageData`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CMessageDataClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CMessageData`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CMessageData%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-071-pathinfo-md"></a>
## PathInfo

- Original corpus path: `12_CLASS_GUIDES/071_pathinfo.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-071-pathinfo-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: pathinfo.h
- `library`: platformenv.lib
- `capability`: None

### `PathInfo`

#### Role

Returns standard public/private media paths.

#### Build metadata

- Header: `pathinfo.h`
- Import library: `platformenv.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <pathinfo.h>
```

```text
LIBRARY         platformenv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use RFs drive information; do not hard-code C: or E:.

#### Compile-time verification

1. Search `pathinfo.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `platformenv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `PathInfo`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/PathInfoClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `PathInfo`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22PathInfo%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-072-sysutil-md"></a>
## SysUtil

- Original corpus path: `12_CLASS_GUIDES/072_sysutil.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-072-sysutil-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: sysutil.h
- `library`: sysutil.lib
- `capability`: None

### `SysUtil`

#### Role

Version and disk-space utility methods.

#### Build metadata

- Header: `sysutil.h`
- Import library: `sysutil.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <sysutil.h>
```

```text
LIBRARY         sysutil.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Disk critical-level checks should precede large writes.

#### Compile-time verification

1. Search `sysutil.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `sysutil.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `SysUtil`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/SysUtilClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `SysUtil`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22SysUtil%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-073-ctextresolver-md"></a>
## CTextResolver

- Original corpus path: `12_CLASS_GUIDES/073_ctextresolver.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-073-ctextresolver-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: textresolver.h
- `library`: commonengine.lib
- `capability`: None

### `CTextResolver`

#### Role

Resolves system error codes to localized text.

#### Build metadata

- Header: `textresolver.h`
- Import library: `commonengine.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <textresolver.h>
```

```text
LIBRARY         commonengine.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use as UI assistance; preserve numeric error in logs.

#### Compile-time verification

1. Search `textresolver.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `commonengine.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CTextResolver`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CTextResolverClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CTextResolver`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CTextResolver%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-074-stringloader-md"></a>
## StringLoader

- Original corpus path: `12_CLASS_GUIDES/074_stringloader.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-074-stringloader-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: stringloader.h
- `library`: commonengine.lib
- `capability`: None

### `StringLoader`

#### Role

Loads and formats localized resource strings.

#### Build metadata

- Header: `stringloader.h`
- Import library: `commonengine.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <stringloader.h>
```

```text
LIBRARY         commonengine.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Choose overload matching %U/%N parameters and cleanup returned HBufC.

#### Compile-time verification

1. Search `stringloader.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `commonengine.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `StringLoader`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/StringLoaderClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `StringLoader`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22StringLoader%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-075-cversioninfo-md"></a>
## CVersionInfo

- Original corpus path: `12_CLASS_GUIDES/075_cversioninfo.md`
- Consolidated reference: `43-class-guides-051-075.md#src-12-class-guides-075-cversioninfo-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: versioninfo.h
- `library`: platformenv.lib
- `capability`: None

### `CVersionInfo`

#### Role

Reads platform/system version information.

#### Build metadata

- Header: `versioninfo.h`
- Import library: `platformenv.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <versioninfo.h>
```

```text
LIBRARY         platformenv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use version checks only when feature detection cannot answer the question.

#### Compile-time verification

1. Search `versioninfo.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `platformenv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CVersionInfo`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CVersionInfoClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CVersionInfo`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CVersionInfo%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.
