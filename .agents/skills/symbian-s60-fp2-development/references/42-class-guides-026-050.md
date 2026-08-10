# Common class guides 026–050

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-12-class-guides-026-caknviewappui-md"></a>
## CAknViewAppUi

- Original corpus path: `12_CLASS_GUIDES/026_caknviewappui.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-026-caknviewappui-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknviewappui.h
- `library`: avkon.lib
- `capability`: None

### `CAknViewAppUi`

#### Role

AppUi with View Framework management.

#### Build metadata

- Header: `aknviewappui.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknviewappui.h>
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

Register views, set default view, activate by TUid.

#### Compile-time verification

1. Search `aknviewappui.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknViewAppUi`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknViewAppUiClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknViewAppUi`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknViewAppUi%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-027-ceikmenupane-md"></a>
## CEikMenuPane

- Original corpus path: `12_CLASS_GUIDES/027_ceikmenupane.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-027-ceikmenupane-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: eikmenup.h
- `library`: local-sdk-check
- `capability`: None

### `CEikMenuPane`

#### Role

Runtime menu pane.

#### Build metadata

- Header: `eikmenup.h`
- Import library: `local-sdk-check`
- Capability: `None`
- Confidence: `verified-header`

```cpp
#include <eikmenup.h>
```

```text
; Resolve import library from local SDK/class page
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Use DynInitMenuPaneL() to dim, add or remove items; library must be confirmed locally.

#### Compile-time verification

1. Search `eikmenup.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `local-sdk-check` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CEikMenuPane`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CEikMenuPaneClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CEikMenuPane`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CEikMenuPane%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-028-caknmessagequerydialog-md"></a>
## CAknMessageQueryDialog

- Original corpus path: `12_CLASS_GUIDES/028_caknmessagequerydialog.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-028-caknmessagequerydialog-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknmessagequerydialog.h
- `library`: avkon.lib
- `capability`: None

### `CAknMessageQueryDialog`

#### Role

Message query dialog.

#### Build metadata

- Header: `aknmessagequerydialog.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknmessagequerydialog.h>
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

PrepareLC()/RunLD() ownership differs from ordinary NewL patterns.

#### Compile-time verification

1. Search `aknmessagequerydialog.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknMessageQueryDialog`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknMessageQueryDialogClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknMessageQueryDialog`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknMessageQueryDialog%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-029-caknquerydialog-md"></a>
## CAknQueryDialog

- Original corpus path: `12_CLASS_GUIDES/029_caknquerydialog.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-029-caknquerydialog-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknquerydialog.h
- `library`: avkon.lib
- `capability`: None

### `CAknQueryDialog`

#### Role

Typed Avkon query dialog.

#### Build metadata

- Header: `aknquerydialog.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknquerydialog.h>
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

Resource type must match the bound data and prompt.

#### Compile-time verification

1. Search `aknquerydialog.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknQueryDialog`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknQueryDialogClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknQueryDialog`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknQueryDialog%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-030-caknsinglestylelistbox-md"></a>
## CAknSingleStyleListBox

- Original corpus path: `12_CLASS_GUIDES/030_caknsinglestylelistbox.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-030-caknsinglestylelistbox-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknlists.h
- `library`: avkon.lib
- `capability`: None

### `CAknSingleStyleListBox`

#### Role

Single-line list box style.

#### Build metadata

- Header: `aknlists.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknlists.h>
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

Own model arrays correctly and call HandleItemAdditionL()/DrawNow() after updates.

#### Compile-time verification

1. Search `aknlists.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknSingleStyleListBox`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknSingleStyleListBoxClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknSingleStyleListBox`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknSingleStyleListBox%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-031-caknsettingitemlist-md"></a>
## CAknSettingItemList

- Original corpus path: `12_CLASS_GUIDES/031_caknsettingitemlist.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-031-caknsettingitemlist-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: aknsettingitemlist.h
- `library`: avkon.lib
- `capability`: None

### `CAknSettingItemList`

#### Role

Container for setting items.

#### Build metadata

- Header: `aknsettingitemlist.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <aknsettingitemlist.h>
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

CreateSettingItemL() binds IDs to backing fields; StoreSettingsL() persists edits.

#### Compile-time verification

1. Search `aknsettingitemlist.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CAknSettingItemList`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CAknSettingItemListClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CAknSettingItemList`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CAknSettingItemList%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-032-rwssession-md"></a>
## RWsSession

- Original corpus path: `12_CLASS_GUIDES/032_rwssession.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-032-rwssession-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: w32std.h
- `library`: ws32.lib
- `capability`: None

### `RWsSession`

#### Role

Window Server client session.

#### Build metadata

- Header: `w32std.h`
- Import library: `ws32.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <w32std.h>
```

```text
LIBRARY         ws32.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect before constructing windows/window groups; Flush() only when needed.

#### Compile-time verification

1. Search `w32std.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `ws32.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RWsSession`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RWsSessionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RWsSession`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RWsSession%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-033-rwindowgroup-md"></a>
## RWindowGroup

- Original corpus path: `12_CLASS_GUIDES/033_rwindowgroup.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-033-rwindowgroup-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: w32std.h
- `library`: ws32.lib
- `capability`: None

### `RWindowGroup`

#### Role

Top-level window group.

#### Build metadata

- Header: `w32std.h`
- Import library: `ws32.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <w32std.h>
```

```text
LIBRARY         ws32.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Construct with RWsSession, set ordinal/name as needed, then Close().

#### Compile-time verification

1. Search `w32std.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `ws32.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RWindowGroup`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RWindowGroupClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RWindowGroup`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RWindowGroup%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-034-cfbsbitmap-md"></a>
## CFbsBitmap

- Original corpus path: `12_CLASS_GUIDES/034_cfbsbitmap.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-034-cfbsbitmap-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: fbs.h
- `library`: fbscli.lib
- `capability`: None

### `CFbsBitmap`

#### Role

Font and Bitmap Server bitmap handle.

#### Build metadata

- Header: `fbs.h`
- Import library: `fbscli.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <fbs.h>
```

```text
LIBRARY         fbscli.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect FBS in non-UI threads; duplicate handles carefully.

#### Compile-time verification

1. Search `fbs.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `fbscli.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CFbsBitmap`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CFbsBitmapClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CFbsBitmap`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CFbsBitmap%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-035-akniconutils-md"></a>
## AknIconUtils

- Original corpus path: `12_CLASS_GUIDES/035_akniconutils.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-035-akniconutils-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: akniconutils.h
- `library`: avkon.lib
- `capability`: None

### `AknIconUtils`

#### Role

Loads and scales platform icons.

#### Build metadata

- Header: `akniconutils.h`
- Import library: `avkon.lib`
- Capability: `None`
- Confidence: `verified-quickref`

```cpp
#include <akniconutils.h>
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

Preserve aspect ratio policy and ownership of bitmap/mask.

#### Compile-time verification

1. Search `akniconutils.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `avkon.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `AknIconUtils`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AknIconUtilsClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `AknIconUtils`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22AknIconUtils%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-036-rsocketserv-md"></a>
## RSocketServ

- Original corpus path: `12_CLASS_GUIDES/036_rsocketserv.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-036-rsocketserv-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: es_sock.h
- `library`: esock.lib
- `capability`: NetworkServices for network use

### `RSocketServ`

#### Role

Socket server session.

#### Build metadata

- Header: `es_sock.h`
- Import library: `esock.lib`
- Capability: `NetworkServices for network use`
- Confidence: `verified-common`

```cpp
#include <es_sock.h>
```

```text
LIBRARY         esock.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Connect before opening sockets/resolvers/connections; close children first.

#### Compile-time verification

1. Search `es_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `esock.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RSocketServ`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RSocketServClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RSocketServ`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RSocketServ%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-037-rsocket-md"></a>
## RSocket

- Original corpus path: `12_CLASS_GUIDES/037_rsocket.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-037-rsocket-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: es_sock.h
- `library`: esock.lib
- `capability`: NetworkServices

### `RSocket`

#### Role

Asynchronous socket handle.

#### Build metadata

- Header: `es_sock.h`
- Import library: `esock.lib`
- Capability: `NetworkServices`
- Confidence: `verified-common`

```cpp
#include <es_sock.h>
```

```text
LIBRARY         esock.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

All connect/read/write requests require active-object or wait handling and cancellation.

#### Compile-time verification

1. Search `es_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `esock.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RSocket`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RSocketClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RSocket`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RSocket%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-038-rconnection-md"></a>
## RConnection

- Original corpus path: `12_CLASS_GUIDES/038_rconnection.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-038-rconnection-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: es_sock.h
- `library`: esock.lib
- `capability`: NetworkServices

### `RConnection`

#### Role

Explicit network connection/IAP handle.

#### Build metadata

- Header: `es_sock.h`
- Import library: `esock.lib`
- Capability: `NetworkServices`
- Confidence: `verified-common`

```cpp
#include <es_sock.h>
```

```text
LIBRARY         esock.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Start() may prompt or use preferences; attach sockets/HTTP to the intended connection.

#### Compile-time verification

1. Search `es_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `esock.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RConnection`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RConnectionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RConnection`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RConnection%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-039-rhostresolver-md"></a>
## RHostResolver

- Original corpus path: `12_CLASS_GUIDES/039_rhostresolver.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-039-rhostresolver-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: in_sock.h
- `library`: esock.lib, insock.lib
- `capability`: NetworkServices

### `RHostResolver`

#### Role

DNS/name resolver.

#### Build metadata

- Header: `in_sock.h`
- Import library: `esock.lib, insock.lib`
- Capability: `NetworkServices`
- Confidence: `verified-common`

```cpp
#include <in_sock.h>
```

```text
LIBRARY         esock.lib
LIBRARY         insock.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Open with address family/protocol; GetByName is asynchronous.

#### Compile-time verification

1. Search `in_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `esock.lib, insock.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RHostResolver`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RHostResolverClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RHostResolver`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RHostResolver%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-040-tinetaddr-md"></a>
## TInetAddr

- Original corpus path: `12_CLASS_GUIDES/040_tinetaddr.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-040-tinetaddr-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: in_sock.h
- `library`: insock.lib
- `capability`: None

### `TInetAddr`

#### Role

IPv4/IPv6 socket address value type.

#### Build metadata

- Header: `in_sock.h`
- Import library: `insock.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <in_sock.h>
```

```text
LIBRARY         insock.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

SetAddress()/SetPort(); avoid assuming host byte order.

#### Compile-time verification

1. Search `in_sock.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `insock.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `TInetAddr`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/TInetAddrClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `TInetAddr`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22TInetAddr%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-041-rhttpsession-md"></a>
## RHTTPSession

- Original corpus path: `12_CLASS_GUIDES/041_rhttpsession.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-041-rhttpsession-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: http.h
- `library`: http.lib
- `capability`: NetworkServices

### `RHTTPSession`

#### Role

HTTP framework session.

#### Build metadata

- Header: `http.h`
- Import library: `http.lib`
- Capability: `NetworkServices`
- Confidence: `verified-common`

```cpp
#include <http.h>
```

```text
LIBRARY         http.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

OpenL(), install filters/properties, open transactions and close all handles.

#### Compile-time verification

1. Search `http.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `http.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RHTTPSession`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RHTTPSessionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RHTTPSession`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RHTTPSession%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-042-rhttptransaction-md"></a>
## RHTTPTransaction

- Original corpus path: `12_CLASS_GUIDES/042_rhttptransaction.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-042-rhttptransaction-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: http.h
- `library`: http.lib
- `capability`: NetworkServices

### `RHTTPTransaction`

#### Role

One HTTP request/response transaction.

#### Build metadata

- Header: `http.h`
- Import library: `http.lib`
- Capability: `NetworkServices`
- Confidence: `verified-common`

```cpp
#include <http.h>
```

```text
LIBRARY         http.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

SubmitL(), consume body data in callback, call ReleaseData(), close transaction.

#### Compile-time verification

1. Search `http.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `http.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `RHTTPTransaction`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/RHTTPTransactionClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `RHTTPTransaction`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22RHTTPTransaction%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-043-mhttptransactioncallback-md"></a>
## MHTTPTransactionCallback

- Original corpus path: `12_CLASS_GUIDES/043_mhttptransactioncallback.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-043-mhttptransactioncallback-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: http.h
- `library`: http.lib
- `capability`: NetworkServices

### `MHTTPTransactionCallback`

#### Role

Receives HTTP transaction events.

#### Build metadata

- Header: `http.h`
- Import library: `http.lib`
- Capability: `NetworkServices`
- Confidence: `verified-common`

```cpp
#include <http.h>
```

```text
LIBRARY         http.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Handle headers, body data, success/failure and errors without blocking scheduler.

#### Compile-time verification

1. Search `http.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `http.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `MHTTPTransactionCallback`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/MHTTPTransactionCallbackClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `MHTTPTransactionCallback`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22MHTTPTransactionCallback%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-044-curi8-md"></a>
## CUri8

- Original corpus path: `12_CLASS_GUIDES/044_curi8.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-044-curi8-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: uri8.h
- `library`: inetprotutil.lib
- `capability`: None

### `CUri8`

#### Role

Parsed 8-bit URI container.

#### Build metadata

- Header: `uri8.h`
- Import library: `inetprotutil.lib`
- Capability: `None`
- Confidence: `verified-common`

```cpp
#include <uri8.h>
```

```text
LIBRARY         inetprotutil.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Prefer URI parser/builders over manual URL slicing.

#### Compile-time verification

1. Search `uri8.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `inetprotutil.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CUri8`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CUri8Class.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CUri8`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CUri8%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-045-cmdaaudioplayerutility-md"></a>
## CMdaAudioPlayerUtility

- Original corpus path: `12_CLASS_GUIDES/045_cmdaaudioplayerutility.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-045-cmdaaudioplayerutility-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: MdaAudioSamplePlayer.h
- `library`: mediaclientaudio.lib
- `capability`: UserEnvironment/device-policy

### `CMdaAudioPlayerUtility`

#### Role

High-level asynchronous audio playback.

#### Build metadata

- Header: `MdaAudioSamplePlayer.h`
- Import library: `mediaclientaudio.lib`
- Capability: `UserEnvironment/device-policy`
- Confidence: `verified-common`

```cpp
#include <MdaAudioSamplePlayer.h>
```

```text
LIBRARY         mediaclientaudio.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

NewL(observer), OpenFileL()/OpenUrlL(), wait for init callback, then Play().

#### Compile-time verification

1. Search `MdaAudioSamplePlayer.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `mediaclientaudio.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CMdaAudioPlayerUtility`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CMdaAudioPlayerUtilityClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CMdaAudioPlayerUtility`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CMdaAudioPlayerUtility%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-046-cmdaaudiorecorderutility-md"></a>
## CMdaAudioRecorderUtility

- Original corpus path: `12_CLASS_GUIDES/046_cmdaaudiorecorderutility.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-046-cmdaaudiorecorderutility-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: MdaAudioSampleEditor.h
- `library`: mediaclientaudio.lib
- `capability`: UserEnvironment/device-policy

### `CMdaAudioRecorderUtility`

#### Role

High-level audio recording and playback.

#### Build metadata

- Header: `MdaAudioSampleEditor.h`
- Import library: `mediaclientaudio.lib`
- Capability: `UserEnvironment/device-policy`
- Confidence: `verified-common`

```cpp
#include <MdaAudioSampleEditor.h>
```

```text
LIBRARY         mediaclientaudio.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

State changes arrive through MoscoStateChangeEvent(); handle permission and format support.

#### Compile-time verification

1. Search `MdaAudioSampleEditor.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `mediaclientaudio.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CMdaAudioRecorderUtility`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CMdaAudioRecorderUtilityClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CMdaAudioRecorderUtility`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CMdaAudioRecorderUtility%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-047-cvideoplayerutility-md"></a>
## CVideoPlayerUtility

- Original corpus path: `12_CLASS_GUIDES/047_cvideoplayerutility.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-047-cvideoplayerutility-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: videoplayer.h
- `library`: mediaclientvideo.lib
- `capability`: UserEnvironment/device-policy

### `CVideoPlayerUtility`

#### Role

High-level video playback.

#### Build metadata

- Header: `videoplayer.h`
- Import library: `mediaclientvideo.lib`
- Capability: `UserEnvironment/device-policy`
- Confidence: `verified-common`

```cpp
#include <videoplayer.h>
```

```text
LIBRARY         mediaclientvideo.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

OpenFileL(), Prepare(), Play(); set display/window rectangles with APIs present in local SDK.

#### Compile-time verification

1. Search `videoplayer.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `mediaclientvideo.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CVideoPlayerUtility`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CVideoPlayerUtilityClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CVideoPlayerUtility`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CVideoPlayerUtility%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-048-ccamera-md"></a>
## CCamera

- Original corpus path: `12_CLASS_GUIDES/048_ccamera.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-048-ccamera-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: ecam.h
- `library`: ecam.lib
- `capability`: UserEnvironment/device-policy

### `CCamera`

#### Role

Camera reservation, capture and viewfinder API.

#### Build metadata

- Header: `ecam.h`
- Import library: `ecam.lib`
- Capability: `UserEnvironment/device-policy`
- Confidence: `verified-common`

```cpp
#include <ecam.h>
```

```text
LIBRARY         ecam.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Reserve and power on asynchronously; camera features vary strongly by device.

#### Compile-time verification

1. Search `ecam.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `ecam.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CCamera`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CCameraClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CCamera`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CCamera%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-049-cimagedecoder-md"></a>
## CImageDecoder

- Original corpus path: `12_CLASS_GUIDES/049_cimagedecoder.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-049-cimagedecoder-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: imageconversion.h
- `library`: imageconversion.lib
- `capability`: path-specific

### `CImageDecoder`

#### Role

Asynchronous image decoder.

#### Build metadata

- Header: `imageconversion.h`
- Import library: `imageconversion.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <imageconversion.h>
```

```text
LIBRARY         imageconversion.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

FileNewL()/DataNewL(), inspect frame info, Convert() to bitmap and handle RunL callback.

#### Compile-time verification

1. Search `imageconversion.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `imageconversion.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CImageDecoder`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CImageDecoderClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CImageDecoder`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CImageDecoder%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.

---

<a id="src-12-class-guides-050-cimageencoder-md"></a>
## CImageEncoder

- Original corpus path: `12_CLASS_GUIDES/050_cimageencoder.md`
- Consolidated reference: `42-class-guides-026-050.md#src-12-class-guides-050-cimageencoder-md`
- `kind`: class_guide
- `target`: Symbian OS 9.3 / S60 3rd FP2
- `header`: imageconversion.h
- `library`: imageconversion.lib
- `capability`: path-specific

### `CImageEncoder`

#### Role

Asynchronous image encoder.

#### Build metadata

- Header: `imageconversion.h`
- Import library: `imageconversion.lib`
- Capability: `path-specific`
- Confidence: `verified-common`

```cpp
#include <imageconversion.h>
```

```text
LIBRARY         imageconversion.lib
```

#### Ownership and error model

- Check whether construction follows `NewL()` / `NewLC()` or an `R`-class `Open`/`Close`
  pattern.
- Any method ending in `L` may leave.
- Any asynchronous method requires a live `TRequestStatus`, cancellation path and active
  scheduler integration.
- Descriptors passed by reference must outlive asynchronous requests.

#### Critical note

Choose encoder UID/MIME and frame data; complete output before releasing source bitmap.

#### Compile-time verification

1. Search `imageconversion.h` under `%EPOCROOT%epoc32\include`.
2. Confirm the exact declaration and conditional compilation guards.
3. Confirm `imageconversion.lib` exists or replace it with the class page/SDK example result.
4. Build a minimal `WINSCW UDEB` and `GCCE UREL` probe.
5. Check capability errors on device; emulator behavior is not sufficient.

#### Agent answer contract

When generating code with `CImageEncoder`, state:

- who owns the object/handle;
- which operation may leave;
- which callback completes asynchronous work;
- how cancellation and teardown work;
- the exact MMP library and capabilities, or clearly label them as local checks.

#### Official reference links

- [Candidate direct class page](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/CImageEncoderClass.html)
- [Class/data-structure index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [All member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Search locator for `CImageEncoder`](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22CImageEncoder%22)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

The candidate class URL follows the mirror's common Doxygen naming. If it does not open,
use the class index or search locator; do not treat a 404 as proof the class is absent.
