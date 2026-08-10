# Common class guides 001–025

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-12-class-guides-001-cbase-md"></a>
## CBase

- Original corpus path: `12_CLASS_GUIDES/001_cbase.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-001-cbase-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32base.h
- `library`: euser.lib
- `capability`: None

### `CBase`

#### Role

Base for heap classes; zero-fills memory and supports Symbian allocation conventions.

#### Build metadata

- Header: `e32base.h`
- Import library: `euser.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <e32base.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Destructor must normally be virtual through the owning interface.

#### Compile-time verification

1. Search `e32base.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CBase`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CBaseClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CBase`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CBase%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-002-cleanupstack-md"></a>
## CleanupStack

- Original corpus path: `12_CLASS_GUIDES/002_cleanupstack.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-002-cleanupstack-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32base.h
- `library`: euser.lib
- `capability`: None

### `CleanupStack`

#### Role

Protects owned resources when a leave unwinds the stack.

#### Build metadata

- Header: `e32base.h`
- Import library: `euser.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <e32base.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Push only objects/resources with the correct cleanup operation; balance every push.

#### Compile-time verification

1. Search `e32base.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CleanupStack`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CleanupStackClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CleanupStack`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CleanupStack%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-003-cactive-md"></a>
## CActive

- Original corpus path: `12_CLASS_GUIDES/003_cactive.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-003-cactive-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32base.h
- `library`: euser.lib
- `capability`: None

### `CActive`

#### Role

Base class for asynchronous request handlers.

#### Build metadata

- Header: `e32base.h`
- Import library: `euser.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <e32base.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Issue request, set iStatus, call SetActive(), implement RunL() and DoCancel().

#### Compile-time verification

1. Search `e32base.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CActive`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CActiveClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CActive`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CActive%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-004-cactivescheduler-md"></a>
## CActiveScheduler

- Original corpus path: `12_CLASS_GUIDES/004_cactivescheduler.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-004-cactivescheduler-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32base.h
- `library`: euser.lib
- `capability`: None

### `CActiveScheduler`

#### Role

Dispatches completed active objects on a thread.

#### Build metadata

- Header: `e32base.h`
- Import library: `euser.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <e32base.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Install exactly one scheduler per thread that uses active objects.

#### Compile-time verification

1. Search `e32base.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CActiveScheduler`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CActiveSchedulerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CActiveScheduler`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CActiveScheduler%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-005-ctimer-md"></a>
## CTimer

- Original corpus path: `12_CLASS_GUIDES/005_ctimer.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-005-ctimer-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32base.h
- `library`: euser.lib
- `capability`: None

### `CTimer`

#### Role

Active-object timer built on RTimer.

#### Build metadata

- Header: `e32base.h`
- Import library: `euser.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <e32base.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

ConstructL() before After()/At(); cancellation is asynchronous-safe through CActive.

#### Compile-time verification

1. Search `e32base.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CTimer`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CTimerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CTimer`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CTimer%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-006-rtimer-md"></a>
## RTimer

- Original corpus path: `12_CLASS_GUIDES/006_rtimer.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-006-rtimer-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32std.h
- `library`: euser.lib
- `capability`: None

### `RTimer`

#### Role

Kernel timer handle.

#### Build metadata

- Header: `e32std.h`
- Import library: `euser.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <e32std.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

CreateLocal(), issue After()/At() with TRequestStatus, then Close().

#### Compile-time verification

1. Search `e32std.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RTimer`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RTimerClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RTimer`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RTimer%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-007-rprocess-md"></a>
## RProcess

- Original corpus path: `12_CLASS_GUIDES/007_rprocess.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-007-rprocess-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32std.h
- `library`: euser.lib
- `capability`: operation-specific

### `RProcess`

#### Role

Process handle and process-launch API.

#### Build metadata

- Header: `e32std.h`
- Import library: `euser.lib`
- Capability: `operation-specific`
- Confidence: `verified-common`

```cpp
#include <e32std.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use Create(), Rendezvous()/Resume() pattern; capabilities affect privileged operations.

#### Compile-time verification

1. Search `e32std.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RProcess`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RProcessClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RProcess`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RProcess%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-008-rthread-md"></a>
## RThread

- Original corpus path: `12_CLASS_GUIDES/008_rthread.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-008-rthread-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32std.h
- `library`: euser.lib
- `capability`: operation-specific

### `RThread`

#### Role

Thread handle and thread-creation API.

#### Build metadata

- Header: `e32std.h`
- Import library: `euser.lib`
- Capability: `operation-specific`
- Confidence: `verified-common`

```cpp
#include <e32std.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Share as little state as possible; heap/cleanup behavior differs by thread.

#### Compile-time verification

1. Search `e32std.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RThread`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RThreadClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RThread`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RThread%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-009-rproperty-md"></a>
## RProperty

- Original corpus path: `12_CLASS_GUIDES/009_rproperty.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-009-rproperty-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: e32property.h
- `library`: euser.lib
- `capability`: policy-specific

### `RProperty`

#### Role

Publish & Subscribe property client.

#### Build metadata

- Header: `e32property.h`
- Import library: `euser.lib`
- Capability: `policy-specific`
- Confidence: `verified-common`

```cpp
#include <e32property.h>
```

```text
LIBRARY         euser.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Read/write permission is defined by the property security policy.

#### Compile-time verification

1. Search `e32property.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `euser.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RProperty`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RPropertyClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RProperty`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RProperty%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-010-rfs-md"></a>
## RFs

- Original corpus path: `12_CLASS_GUIDES/010_rfs.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-010-rfs-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: f32file.h
- `library`: efsrv.lib
- `capability`: path-specific

### `RFs`

#### Role

Session to the file server.

#### Build metadata

- Header: `f32file.h`
- Import library: `efsrv.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <f32file.h>
```

```text
LIBRARY         efsrv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect() once per owner; CleanupClosePushL or explicit Close().

#### Compile-time verification

1. Search `f32file.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `efsrv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RFs`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RFsClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RFs`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RFs%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-011-rfile-md"></a>
## RFile

- Original corpus path: `12_CLASS_GUIDES/011_rfile.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-011-rfile-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: f32file.h
- `library`: efsrv.lib
- `capability`: path-specific

### `RFile`

#### Role

Open file handle.

#### Build metadata

- Header: `f32file.h`
- Import library: `efsrv.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <f32file.h>
```

```text
LIBRARY         efsrv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Check sharing flags; flush/close deterministically.

#### Compile-time verification

1. Search `f32file.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `efsrv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RFile`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RFileClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RFile`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RFile%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-012-cfileman-md"></a>
## CFileMan

- Original corpus path: `12_CLASS_GUIDES/012_cfileman.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-012-cfileman-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: f32file.h
- `library`: efsrv.lib
- `capability`: path-specific

### `CFileMan`

#### Role

High-level copy, move, rename and delete operations.

#### Build metadata

- Header: `f32file.h`
- Import library: `efsrv.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <f32file.h>
```

```text
LIBRARY         efsrv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Operations may recurse and can be asynchronous depending on overload.

#### Compile-time verification

1. Search `f32file.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `efsrv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CFileMan`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CFileManClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CFileMan`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CFileMan%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-013-tparse-md"></a>
## TParse

- Original corpus path: `12_CLASS_GUIDES/013_tparse.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-013-tparse-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: f32file.h
- `library`: efsrv.lib
- `capability`: None

### `TParse`

#### Role

Parses and composes Symbian file names.

#### Build metadata

- Header: `f32file.h`
- Import library: `efsrv.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <f32file.h>
```

```text
LIBRARY         efsrv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use drive/path/name/ext components instead of manual string concatenation.

#### Compile-time verification

1. Search `f32file.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `efsrv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `TParse`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/TParseClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `TParse`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22TParse%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-014-rreadstream-md"></a>
## RReadStream

- Original corpus path: `12_CLASS_GUIDES/014_rreadstream.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-014-rreadstream-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: s32strm.h
- `library`: estor.lib
- `capability`: source-specific

### `RReadStream`

#### Role

Binary read stream.

#### Build metadata

- Header: `s32strm.h`
- Import library: `estor.lib`
- Capability: `source-specific`
- Confidence: `verified-common`

```cpp
#include <s32strm.h>
```

```text
LIBRARY         estor.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Externalize/Internalize formats must stay version-compatible.

#### Compile-time verification

1. Search `s32strm.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `estor.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RReadStream`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RReadStreamClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RReadStream`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RReadStream%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-015-rwritestream-md"></a>
## RWriteStream

- Original corpus path: `12_CLASS_GUIDES/015_rwritestream.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-015-rwritestream-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: s32strm.h
- `library`: estor.lib
- `capability`: sink-specific

### `RWriteStream`

#### Role

Binary write stream.

#### Build metadata

- Header: `s32strm.h`
- Import library: `estor.lib`
- Capability: `sink-specific`
- Confidence: `verified-common`

```cpp
#include <s32strm.h>
```

```text
LIBRARY         estor.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Call CommitL() where required before Close().

#### Compile-time verification

1. Search `s32strm.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `estor.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RWriteStream`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RWriteStreamClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RWriteStream`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RWriteStream%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-016-rfilereadstream-md"></a>
## RFileReadStream

- Original corpus path: `12_CLASS_GUIDES/016_rfilereadstream.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-016-rfilereadstream-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: s32file.h
- `library`: estor.lib, efsrv.lib
- `capability`: path-specific

### `RFileReadStream`

#### Role

Read stream backed by RFile/RFs.

#### Build metadata

- Header: `s32file.h`
- Import library: `estor.lib, efsrv.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <s32file.h>
```

```text
LIBRARY         estor.lib
LIBRARY         efsrv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Push with CleanupClosePushL after successful open.

#### Compile-time verification

1. Search `s32file.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `estor.lib, efsrv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RFileReadStream`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RFileReadStreamClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RFileReadStream`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RFileReadStream%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-017-rfilewritestream-md"></a>
## RFileWriteStream

- Original corpus path: `12_CLASS_GUIDES/017_rfilewritestream.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-017-rfilewritestream-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: s32file.h
- `library`: estor.lib, efsrv.lib
- `capability`: path-specific

### `RFileWriteStream`

#### Role

Write stream backed by RFile/RFs.

#### Build metadata

- Header: `s32file.h`
- Import library: `estor.lib, efsrv.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <s32file.h>
```

```text
LIBRARY         estor.lib
LIBRARY         efsrv.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Replace/Create/Open semantics differ; commit before cleanup.

#### Compile-time verification

1. Search `s32file.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `estor.lib, efsrv.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RFileWriteStream`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RFileWriteStreamClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RFileWriteStream`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RFileWriteStream%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-018-rdbnameddatabase-md"></a>
## RDbNamedDatabase

- Original corpus path: `12_CLASS_GUIDES/018_rdbnameddatabase.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-018-rdbnameddatabase-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: d32dbms.h
- `library`: edbms.lib
- `capability`: path-specific

### `RDbNamedDatabase`

#### Role

DBMS database file session.

#### Build metadata

- Header: `d32dbms.h`
- Import library: `edbms.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <d32dbms.h>
```

```text
LIBRARY         edbms.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use transactions for multi-row modifications and recover on errors.

#### Compile-time verification

1. Search `d32dbms.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `edbms.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RDbNamedDatabase`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RDbNamedDatabaseClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RDbNamedDatabase`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RDbNamedDatabase%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-019-rdbview-md"></a>
## RDbView

- Original corpus path: `12_CLASS_GUIDES/019_rdbview.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-019-rdbview-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: d32dbms.h
- `library`: edbms.lib
- `capability`: database-specific

### `RDbView`

#### Role

Prepared DBMS query/update view.

#### Build metadata

- Header: `d32dbms.h`
- Import library: `edbms.lib`
- Capability: `database-specific`
- Confidence: `verified-common`

```cpp
#include <d32dbms.h>
```

```text
LIBRARY         edbms.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Prepare, EvaluateAll(), navigate rows, GetL()/UpdateL(), then Close().

#### Compile-time verification

1. Search `d32dbms.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `edbms.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RDbView`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RDbViewClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RDbView`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RDbView%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-020-crepository-md"></a>
## CRepository

- Original corpus path: `12_CLASS_GUIDES/020_crepository.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-020-crepository-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: centralrepository.h
- `library`: centralrepository.lib
- `capability`: key-policy-specific

### `CRepository`

#### Role

Central Repository client for typed settings.

#### Build metadata

- Header: `centralrepository.h`
- Import library: `centralrepository.lib`
- Capability: `key-policy-specific`
- Confidence: `verified-common`

```cpp
#include <centralrepository.h>
```

```text
LIBRARY         centralrepository.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Repository UID and key policies determine access; notifications are one-shot.

#### Compile-time verification

1. Search `centralrepository.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `centralrepository.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CRepository`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CRepositoryClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CRepository`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CRepository%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-021-ccoecontrol-md"></a>
## CCoeControl

- Original corpus path: `12_CLASS_GUIDES/021_ccoecontrol.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-021-ccoecontrol-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: coecntrl.h
- `library`: cone.lib
- `capability`: None

### `CCoeControl`

#### Role

Base class for window-owning or compound controls.

#### Build metadata

- Header: `coecntrl.h`
- Import library: `cone.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <coecntrl.h>
```

```text
LIBRARY         cone.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Implement Draw(), SizeChanged(), CountComponentControls()/ComponentControl() as needed.

#### Compile-time verification

1. Search `coecntrl.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `cone.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CCoeControl`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CCoeControlClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CCoeControl`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CCoeControl%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-022-caknapplication-md"></a>
## CAknApplication

- Original corpus path: `12_CLASS_GUIDES/022_caknapplication.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-022-caknapplication-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknapp.h
- `library`: avkon.lib, apparc.lib
- `capability`: None

### `CAknApplication`

#### Role

Avkon application factory and application UID owner.

#### Build metadata

- Header: `aknapp.h`
- Import library: `avkon.lib, apparc.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknapp.h>
```

```text
LIBRARY         avkon.lib
LIBRARY         apparc.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Implement CreateDocumentL() and AppDllUid().

#### Compile-time verification

1. Search `aknapp.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib, apparc.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknApplication`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknApplicationClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknApplication`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknApplication%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-023-cakndocument-md"></a>
## CAknDocument

- Original corpus path: `12_CLASS_GUIDES/023_cakndocument.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-023-cakndocument-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: akndoc.h
- `library`: avkon.lib
- `capability`: None

### `CAknDocument`

#### Role

Creates the application UI object.

#### Build metadata

- Header: `akndoc.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <akndoc.h>
```

```text
LIBRARY         avkon.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Implement CreateAppUiL(); keep persistent document state separate from controls.

#### Compile-time verification

1. Search `akndoc.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknDocument`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknDocumentClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknDocument`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknDocument%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-024-caknappui-md"></a>
## CAknAppUi

- Original corpus path: `12_CLASS_GUIDES/024_caknappui.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-024-caknappui-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknappui.h
- `library`: avkon.lib, eikcore.lib
- `capability`: None

### `CAknAppUi`

#### Role

Top-level Avkon command and control-stack owner.

#### Build metadata

- Header: `aknappui.h`
- Import library: `avkon.lib, eikcore.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknappui.h>
```

```text
LIBRARY         avkon.lib
LIBRARY         eikcore.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

BaseConstructL(), AddToStackL(), handle exit commands, remove/delete controls.

#### Compile-time verification

1. Search `aknappui.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib, eikcore.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknAppUi`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknAppUiClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknAppUi`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknAppUi%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-025-caknview-md"></a>
## CAknView

- Original corpus path: `12_CLASS_GUIDES/025_caknview.md`
- Consolidated reference: `41-class-guides-001-025.md#src-12-class-guides-025-caknview-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknview.h
- `library`: avkon.lib
- `capability`: None

### `CAknView`

#### Role

View Framework screen abstraction.

#### Build metadata

- Header: `aknview.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknview.h>
```

```text
LIBRARY         avkon.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Implement Id(), DoActivateL(), DoDeactivate(); do not keep stale container pointers.

#### Compile-time verification

1. Search `aknview.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknView`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknViewClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknView`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknView%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.
