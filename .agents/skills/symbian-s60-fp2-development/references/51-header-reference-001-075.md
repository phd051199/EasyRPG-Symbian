# Public header references 001–075

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-20-public-header-reference-001-activeapdb-h-md"></a>
## activeapdb.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/001_activeapdb_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-001-activeapdb-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `activeapdb.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Access Point Engine API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-004-access-point-engine-api-md)

#### Cách include

```cpp
#include <activeapdb.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\activeapdb.h"
findstr /s /n /i /c:"#include <activeapdb.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"activeapdb.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: activeapdb.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <activeapdb.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22activeapdb.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-002-activefavouritesdbnotifier-h-md"></a>
## activefavouritesdbnotifier.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/002_activefavouritesdbnotifier_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-002-activefavouritesdbnotifier-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `activefavouritesdbnotifier.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Favourites Engine API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-038-favourites-engine-api-md)

#### Cách include

```cpp
#include <activefavouritesdbnotifier.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\activefavouritesdbnotifier.h"
findstr /s /n /i /c:"#include <activefavouritesdbnotifier.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"activefavouritesdbnotifier.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: activefavouritesdbnotifier.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <activefavouritesdbnotifier.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22activefavouritesdbnotifier.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-003-aiwcommon-h-md"></a>
## aiwcommon.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/003_aiwcommon_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-003-aiwcommon-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwcommon.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [AIW Criteria API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-001-aiw-criteria-api-md)

#### Cách include

```cpp
#include <aiwcommon.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwcommon.h"
findstr /s /n /i /c:"#include <aiwcommon.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwcommon.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwcommon.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwcommon.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwcommon.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-004-aiwcommon-hrh-md"></a>
## aiwcommon.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/004_aiwcommon_hrh.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-004-aiwcommon-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwcommon.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [AIW Criteria API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-001-aiw-criteria-api-md)

#### Cách include

```cpp
#include <aiwcommon.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwcommon.hrh"
findstr /s /n /i /c:"#include <aiwcommon.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwcommon.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwcommon.hrh
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwcommon.hrh>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwcommon.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-005-aiwgenericparam-h-md"></a>
## aiwgenericparam.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/005_aiwgenericparam_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-005-aiwgenericparam-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwgenericparam.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [AIW Generic Parameter API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md)

#### Cách include

```cpp
#include <aiwgenericparam.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwgenericparam.h"
findstr /s /n /i /c:"#include <aiwgenericparam.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwgenericparam.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwgenericparam.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwgenericparam.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwgenericparam.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-006-aiwgenericparam-hrh-md"></a>
## aiwgenericparam.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/006_aiwgenericparam_hrh.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-006-aiwgenericparam-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwgenericparam.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [AIW Generic Parameter API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md)

#### Cách include

```cpp
#include <aiwgenericparam.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwgenericparam.hrh"
findstr /s /n /i /c:"#include <aiwgenericparam.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwgenericparam.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwgenericparam.hrh
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwgenericparam.hrh>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwgenericparam.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-007-aiwservicehandler-h-md"></a>
## aiwservicehandler.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/007_aiwservicehandler_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-007-aiwservicehandler-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwservicehandler.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [AIW Service Handler API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-003-aiw-service-handler-api-md)

#### Cách include

```cpp
#include <aiwservicehandler.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwservicehandler.h"
findstr /s /n /i /c:"#include <aiwservicehandler.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwservicehandler.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwservicehandler.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwservicehandler.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwservicehandler.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-008-aiwvariant-h-md"></a>
## aiwvariant.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/008_aiwvariant_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-008-aiwvariant-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwvariant.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [AIW Generic Parameter API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md)

#### Cách include

```cpp
#include <aiwvariant.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwvariant.h"
findstr /s /n /i /c:"#include <aiwvariant.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwvariant.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwvariant.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwvariant.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwvariant.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-009-aiwvarianttype-hrh-md"></a>
## aiwvarianttype.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/009_aiwvarianttype_hrh.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-009-aiwvarianttype-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aiwvarianttype.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [AIW Generic Parameter API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md)

#### Cách include

```cpp
#include <aiwvarianttype.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aiwvarianttype.hrh"
findstr /s /n /i /c:"#include <aiwvarianttype.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aiwvarianttype.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aiwvarianttype.hrh
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aiwvarianttype.hrh>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aiwvarianttype.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-010-aknanim-hrh-md"></a>
## aknanim.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/010_aknanim_hrh.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-010-aknanim-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknanim.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Animation API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-006-animation-api-md)

#### Cách include

```cpp
#include <aknanim.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknanim.hrh"
findstr /s /n /i /c:"#include <aknanim.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknanim.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknanim.hrh
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknanim.hrh>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknanim.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-011-aknapp-h-md"></a>
## aknapp.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/011_aknapp_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-011-aknapp-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknapp.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Application Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-007-application-framework-api-md)

#### Cách include

```cpp
#include <aknapp.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknapp.h"
findstr /s /n /i /c:"#include <aknapp.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknapp.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknapp.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknapp.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknapp.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-012-aknappui-h-md"></a>
## aknappui.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/012_aknappui_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-012-aknappui-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknappui.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Application Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-007-application-framework-api-md)

#### Cách include

```cpp
#include <aknappui.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknappui.h"
findstr /s /n /i /c:"#include <aknappui.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknappui.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknappui.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknappui.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknappui.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-013-aknbitmapanimation-h-md"></a>
## aknbitmapanimation.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/013_aknbitmapanimation_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-013-aknbitmapanimation-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknbitmapanimation.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Animation API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-006-animation-api-md)

#### Cách include

```cpp
#include <aknbitmapanimation.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknbitmapanimation.h"
findstr /s /n /i /c:"#include <aknbitmapanimation.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknbitmapanimation.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknbitmapanimation.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknbitmapanimation.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknbitmapanimation.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-014-akncheckboxsettingpage-h-md"></a>
## akncheckboxsettingpage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/014_akncheckboxsettingpage_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-014-akncheckboxsettingpage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akncheckboxsettingpage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Setting Pages API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-100-setting-pages-api-md)

#### Cách include

```cpp
#include <akncheckboxsettingpage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akncheckboxsettingpage.h"
findstr /s /n /i /c:"#include <akncheckboxsettingpage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akncheckboxsettingpage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akncheckboxsettingpage.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akncheckboxsettingpage.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akncheckboxsettingpage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-015-aknclearer-h-md"></a>
## aknclearer.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/015_aknclearer_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-015-aknclearer-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknclearer.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Screen Clearer API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-093-screen-clearer-api-md)

#### Cách include

```cpp
#include <aknclearer.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknclearer.h"
findstr /s /n /i /c:"#include <aknclearer.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknclearer.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknclearer.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknclearer.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknclearer.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-016-akncolourselectiongrid-h-md"></a>
## akncolourselectiongrid.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/016_akncolourselectiongrid_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-016-akncolourselectiongrid-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akncolourselectiongrid.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Grids API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-043-grids-api-md)

#### Cách include

```cpp
#include <akncolourselectiongrid.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akncolourselectiongrid.h"
findstr /s /n /i /c:"#include <akncolourselectiongrid.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akncolourselectiongrid.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akncolourselectiongrid.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akncolourselectiongrid.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akncolourselectiongrid.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-017-akncommondialogs-h-md"></a>
## akncommondialogs.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/017_akncommondialogs_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-017-akncommondialogs-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akncommondialogs.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Common File Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md)

#### Cách include

```cpp
#include <akncommondialogs.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akncommondialogs.h"
findstr /s /n /i /c:"#include <akncommondialogs.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akncommondialogs.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akncommondialogs.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akncommondialogs.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akncommondialogs.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-018-akncommondialogsdynmem-h-md"></a>
## akncommondialogsdynmem.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/018_akncommondialogsdynmem_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-018-akncommondialogsdynmem-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akncommondialogsdynmem.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Common File Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md)

#### Cách include

```cpp
#include <akncommondialogsdynmem.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akncommondialogsdynmem.h"
findstr /s /n /i /c:"#include <akncommondialogsdynmem.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akncommondialogsdynmem.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akncommondialogsdynmem.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akncommondialogsdynmem.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akncommondialogsdynmem.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-019-akncontext-h-md"></a>
## akncontext.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/019_akncontext_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-019-akncontext-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akncontext.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Context Pane API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-025-context-pane-api-md)

#### Cách include

```cpp
#include <akncontext.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akncontext.h"
findstr /s /n /i /c:"#include <akncontext.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akncontext.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akncontext.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akncontext.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akncontext.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-020-akncontrol-h-md"></a>
## akncontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/020_akncontrol_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-020-akncontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akncontrol.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Base Controls API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-011-base-controls-api-md)

#### Cách include

```cpp
#include <akncontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akncontrol.h"
findstr /s /n /i /c:"#include <akncontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akncontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akncontrol.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akncontrol.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akncontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-021-akndescarraydecorator-h-md"></a>
## akndescarraydecorator.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/021_akndescarraydecorator_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-021-akndescarraydecorator-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akndescarraydecorator.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Setting Pages API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-100-setting-pages-api-md)

#### Cách include

```cpp
#include <akndescarraydecorator.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akndescarraydecorator.h"
findstr /s /n /i /c:"#include <akndescarraydecorator.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akndescarraydecorator.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akndescarraydecorator.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akndescarraydecorator.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akndescarraydecorator.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-022-akndialog-h-md"></a>
## akndialog.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/022_akndialog_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-022-akndialog-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akndialog.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)

#### Cách include

```cpp
#include <akndialog.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akndialog.h"
findstr /s /n /i /c:"#include <akndialog.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akndialog.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akndialog.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akndialog.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akndialog.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-023-akndlgshut-h-md"></a>
## akndlgshut.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/023_akndlgshut_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-023-akndlgshut-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akndlgshut.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Dialog Shutter API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-029-dialog-shutter-api-md)

#### Cách include

```cpp
#include <akndlgshut.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akndlgshut.h"
findstr /s /n /i /c:"#include <akndlgshut.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akndlgshut.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akndlgshut.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akndlgshut.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akndlgshut.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-024-akndoc-h-md"></a>
## akndoc.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/024_akndoc_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-024-akndoc-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akndoc.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Application Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-007-application-framework-api-md)

#### Cách include

```cpp
#include <akndoc.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akndoc.h"
findstr /s /n /i /c:"#include <akndoc.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akndoc.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akndoc.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akndoc.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akndoc.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-025-akneditstateindicator-h-md"></a>
## akneditstateindicator.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/025_akneditstateindicator_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-025-akneditstateindicator-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akneditstateindicator.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Indicators API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-048-indicators-api-md)

#### Cách include

```cpp
#include <akneditstateindicator.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akneditstateindicator.h"
findstr /s /n /i /c:"#include <akneditstateindicator.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akneditstateindicator.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akneditstateindicator.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akneditstateindicator.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akneditstateindicator.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-026-aknedsts-h-md"></a>
## aknedsts.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/026_aknedsts_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-026-aknedsts-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknedsts.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknedsts.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknedsts.h"
findstr /s /n /i /c:"#include <aknedsts.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknedsts.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknedsts.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknedsts.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknedsts.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-027-aknedstsobs-h-md"></a>
## aknedstsobs.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/027_aknedstsobs_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-027-aknedstsobs-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknedstsobs.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknedstsobs.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknedstsobs.h"
findstr /s /n /i /c:"#include <aknedstsobs.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknedstsobs.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknedstsobs.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknedstsobs.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknedstsobs.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-028-aknfontaccess-h-md"></a>
## aknfontaccess.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/028_aknfontaccess_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-028-aknfontaccess-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknfontaccess.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Fonts API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-041-fonts-api-md)

#### Cách include

```cpp
#include <aknfontaccess.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknfontaccess.h"
findstr /s /n /i /c:"#include <aknfontaccess.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknfontaccess.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknfontaccess.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknfontaccess.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknfontaccess.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-029-aknform-h-md"></a>
## aknform.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/029_aknform_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-029-aknform-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknform.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Form API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-042-form-api-md)

#### Cách include

```cpp
#include <aknform.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknform.h"
findstr /s /n /i /c:"#include <aknform.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknform.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknform.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknform.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknform.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-030-aknglobalconfirmationquery-h-md"></a>
## aknglobalconfirmationquery.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/030_aknglobalconfirmationquery_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-030-aknglobalconfirmationquery-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknglobalconfirmationquery.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <aknglobalconfirmationquery.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknglobalconfirmationquery.h"
findstr /s /n /i /c:"#include <aknglobalconfirmationquery.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknglobalconfirmationquery.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknglobalconfirmationquery.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknglobalconfirmationquery.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknglobalconfirmationquery.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-031-akngloballistquery-h-md"></a>
## akngloballistquery.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/031_akngloballistquery_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-031-akngloballistquery-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akngloballistquery.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <akngloballistquery.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akngloballistquery.h"
findstr /s /n /i /c:"#include <akngloballistquery.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akngloballistquery.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akngloballistquery.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akngloballistquery.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akngloballistquery.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-032-aknglobalmsgquery-h-md"></a>
## aknglobalmsgquery.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/032_aknglobalmsgquery_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-032-aknglobalmsgquery-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknglobalmsgquery.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <aknglobalmsgquery.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknglobalmsgquery.h"
findstr /s /n /i /c:"#include <aknglobalmsgquery.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknglobalmsgquery.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknglobalmsgquery.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknglobalmsgquery.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknglobalmsgquery.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-033-aknglobalnote-h-md"></a>
## aknglobalnote.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/033_aknglobalnote_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-033-aknglobalnote-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknglobalnote.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <aknglobalnote.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknglobalnote.h"
findstr /s /n /i /c:"#include <aknglobalnote.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknglobalnote.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknglobalnote.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknglobalnote.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknglobalnote.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-034-aknglobalprogressdialog-h-md"></a>
## aknglobalprogressdialog.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/034_aknglobalprogressdialog_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-034-aknglobalprogressdialog-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknglobalprogressdialog.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <aknglobalprogressdialog.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknglobalprogressdialog.h"
findstr /s /n /i /c:"#include <aknglobalprogressdialog.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknglobalprogressdialog.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknglobalprogressdialog.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknglobalprogressdialog.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknglobalprogressdialog.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-035-akngmsstylegrid-h-md"></a>
## akngmsstylegrid.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/035_akngmsstylegrid_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-035-akngmsstylegrid-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akngmsstylegrid.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Grids API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-043-grids-api-md)

#### Cách include

```cpp
#include <akngmsstylegrid.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akngmsstylegrid.h"
findstr /s /n /i /c:"#include <akngmsstylegrid.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akngmsstylegrid.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akngmsstylegrid.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akngmsstylegrid.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akngmsstylegrid.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-036-akngrid-h-md"></a>
## akngrid.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/036_akngrid_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-036-akngrid-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akngrid.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Grids API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-043-grids-api-md)

#### Cách include

```cpp
#include <akngrid.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akngrid.h"
findstr /s /n /i /c:"#include <akngrid.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akngrid.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akngrid.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akngrid.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akngrid.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-037-akngridm-h-md"></a>
## akngridm.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/037_akngridm_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-037-akngridm-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akngridm.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Grids API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-043-grids-api-md)

#### Cách include

```cpp
#include <akngridm.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akngridm.h"
findstr /s /n /i /c:"#include <akngridm.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akngridm.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akngridm.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akngridm.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akngridm.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-038-akngridview-h-md"></a>
## akngridview.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/038_akngridview_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-038-akngridview-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akngridview.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Grids API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-043-grids-api-md)

#### Cách include

```cpp
#include <akngridview.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akngridview.h"
findstr /s /n /i /c:"#include <akngridview.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akngridview.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akngridview.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akngridview.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akngridview.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-039-akniconobserver-h-md"></a>
## akniconobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/039_akniconobserver_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-039-akniconobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akniconobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Scalable Icons API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-092-scalable-icons-api-md)

#### Cách include

```cpp
#include <akniconobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akniconobserver.h"
findstr /s /n /i /c:"#include <akniconobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akniconobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akniconobserver.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akniconobserver.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akniconobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-040-akniconsrvclient-h-md"></a>
## akniconsrvclient.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/040_akniconsrvclient_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-040-akniconsrvclient-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akniconsrvclient.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Scalable Icons API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-092-scalable-icons-api-md)

#### Cách include

```cpp
#include <akniconsrvclient.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akniconsrvclient.h"
findstr /s /n /i /c:"#include <akniconsrvclient.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akniconsrvclient.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akniconsrvclient.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akniconsrvclient.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akniconsrvclient.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-041-akniconutils-h-md"></a>
## akniconutils.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/041_akniconutils_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-041-akniconutils-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akniconutils.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Scalable Icons API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-092-scalable-icons-api-md)

#### Cách include

```cpp
#include <akniconutils.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akniconutils.h"
findstr /s /n /i /c:"#include <akniconutils.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akniconutils.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akniconutils.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akniconutils.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akniconutils.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-042-aknindicatorcontainer-h-md"></a>
## aknindicatorcontainer.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/042_aknindicatorcontainer_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-042-aknindicatorcontainer-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknindicatorcontainer.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Indicators API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-048-indicators-api-md)

#### Cách include

```cpp
#include <aknindicatorcontainer.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknindicatorcontainer.h"
findstr /s /n /i /c:"#include <aknindicatorcontainer.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknindicatorcontainer.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknindicatorcontainer.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknindicatorcontainer.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknindicatorcontainer.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-043-akninfopopupnotecontroller-h-md"></a>
## akninfopopupnotecontroller.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/043_akninfopopupnotecontroller_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-043-akninfopopupnotecontroller-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akninfopopupnotecontroller.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Info Popup Note API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-049-info-popup-note-api-md)

#### Cách include

```cpp
#include <akninfopopupnotecontroller.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akninfopopupnotecontroller.h"
findstr /s /n /i /c:"#include <akninfopopupnotecontroller.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akninfopopupnotecontroller.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akninfopopupnotecontroller.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akninfopopupnotecontroller.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akninfopopupnotecontroller.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-044-akninfrm-h-md"></a>
## akninfrm.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/044_akninfrm_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-044-akninfrm-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akninfrm.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Input Frame API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-052-input-frame-api-md)

#### Cách include

```cpp
#include <akninfrm.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akninfrm.h"
findstr /s /n /i /c:"#include <akninfrm.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akninfrm.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akninfrm.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akninfrm.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akninfrm.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-045-akninputlanguageinfo-h-md"></a>
## akninputlanguageinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/045_akninputlanguageinfo_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-045-akninputlanguageinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `akninputlanguageinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Input Language Info API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-053-input-language-info-api-md)

#### Cách include

```cpp
#include <akninputlanguageinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\akninputlanguageinfo.h"
findstr /s /n /i /c:"#include <akninputlanguageinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"akninputlanguageinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: akninputlanguageinfo.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <akninputlanguageinfo.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22akninputlanguageinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-046-aknintermediate-h-md"></a>
## aknintermediate.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/046_aknintermediate_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-046-aknintermediate-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknintermediate.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Intermediate State API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-054-intermediate-state-api-md)

#### Cách include

```cpp
#include <aknintermediate.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknintermediate.h"
findstr /s /n /i /c:"#include <aknintermediate.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknintermediate.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknintermediate.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknintermediate.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknintermediate.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-047-aknipfed-h-md"></a>
## aknipfed.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/047_aknipfed_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-047-aknipfed-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknipfed.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknipfed.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknipfed.h"
findstr /s /n /i /c:"#include <aknipfed.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknipfed.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknipfed.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknipfed.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknipfed.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-048-aknkeylock-h-md"></a>
## aknkeylock.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/048_aknkeylock_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-048-aknkeylock-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknkeylock.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Keylock API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-055-keylock-api-md)

#### Cách include

```cpp
#include <aknkeylock.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknkeylock.h"
findstr /s /n /i /c:"#include <aknkeylock.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknkeylock.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknkeylock.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknkeylock.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknkeylock.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-049-aknlayoutfont-h-md"></a>
## aknlayoutfont.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/049_aknlayoutfont_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-049-aknlayoutfont-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlayoutfont.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Fonts API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-041-fonts-api-md)

#### Cách include

```cpp
#include <aknlayoutfont.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlayoutfont.h"
findstr /s /n /i /c:"#include <aknlayoutfont.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlayoutfont.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlayoutfont.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlayoutfont.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlayoutfont.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-050-aknlistboxlayoutdecorator-h-md"></a>
## aknlistboxlayoutdecorator.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/050_aknlistboxlayoutdecorator_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-050-aknlistboxlayoutdecorator-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlistboxlayoutdecorator.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <aknlistboxlayoutdecorator.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlistboxlayoutdecorator.h"
findstr /s /n /i /c:"#include <aknlistboxlayoutdecorator.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlistboxlayoutdecorator.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlistboxlayoutdecorator.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlistboxlayoutdecorator.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlistboxlayoutdecorator.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-051-aknlistboxsettingpage-h-md"></a>
## aknlistboxsettingpage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/051_aknlistboxsettingpage_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-051-aknlistboxsettingpage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlistboxsettingpage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <aknlistboxsettingpage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlistboxsettingpage.h"
findstr /s /n /i /c:"#include <aknlistboxsettingpage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlistboxsettingpage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlistboxsettingpage.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlistboxsettingpage.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlistboxsettingpage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-052-aknlistquerycontrol-h-md"></a>
## aknlistquerycontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/052_aknlistquerycontrol_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-052-aknlistquerycontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlistquerycontrol.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)

#### Cách include

```cpp
#include <aknlistquerycontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlistquerycontrol.h"
findstr /s /n /i /c:"#include <aknlistquerycontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlistquerycontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlistquerycontrol.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlistquerycontrol.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlistquerycontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-053-aknlistquerydialog-h-md"></a>
## aknlistquerydialog.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/053_aknlistquerydialog_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-053-aknlistquerydialog-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlistquerydialog.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)

#### Cách include

```cpp
#include <aknlistquerydialog.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlistquerydialog.h"
findstr /s /n /i /c:"#include <aknlistquerydialog.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlistquerydialog.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlistquerydialog.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlistquerydialog.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlistquerydialog.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-054-aknlists-h-md"></a>
## aknlists.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/054_aknlists_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-054-aknlists-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlists.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <aknlists.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlists.h"
findstr /s /n /i /c:"#include <aknlists.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlists.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlists.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlists.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlists.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-055-aknlocationed-h-md"></a>
## aknlocationed.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/055_aknlocationed_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-055-aknlocationed-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknlocationed.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknlocationed.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknlocationed.h"
findstr /s /n /i /c:"#include <aknlocationed.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknlocationed.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknlocationed.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknlocationed.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknlocationed.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-056-aknmessagequerycontrol-h-md"></a>
## aknmessagequerycontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/056_aknmessagequerycontrol_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-056-aknmessagequerycontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknmessagequerycontrol.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)

#### Cách include

```cpp
#include <aknmessagequerycontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknmessagequerycontrol.h"
findstr /s /n /i /c:"#include <aknmessagequerycontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknmessagequerycontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknmessagequerycontrol.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknmessagequerycontrol.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknmessagequerycontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-057-aknmessagequerydialog-h-md"></a>
## aknmessagequerydialog.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/057_aknmessagequerydialog_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-057-aknmessagequerydialog-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknmessagequerydialog.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)

#### Cách include

```cpp
#include <aknmessagequerydialog.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknmessagequerydialog.h"
findstr /s /n /i /c:"#include <aknmessagequerydialog.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknmessagequerydialog.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknmessagequerydialog.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknmessagequerydialog.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknmessagequerydialog.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-058-aknmfnecommandobserver-h-md"></a>
## aknmfnecommandobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/058_aknmfnecommandobserver_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-058-aknmfnecommandobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknmfnecommandobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknmfnecommandobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknmfnecommandobserver.h"
findstr /s /n /i /c:"#include <aknmfnecommandobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknmfnecommandobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknmfnecommandobserver.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknmfnecommandobserver.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknmfnecommandobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-059-aknmfnesettingpage-h-md"></a>
## aknmfnesettingpage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/059_aknmfnesettingpage_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-059-aknmfnesettingpage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknmfnesettingpage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Setting Pages API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-100-setting-pages-api-md)

#### Cách include

```cpp
#include <aknmfnesettingpage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknmfnesettingpage.h"
findstr /s /n /i /c:"#include <aknmfnesettingpage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknmfnesettingpage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknmfnesettingpage.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknmfnesettingpage.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknmfnesettingpage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-060-aknmultilinequerycontrol-h-md"></a>
## aknmultilinequerycontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/060_aknmultilinequerycontrol_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-060-aknmultilinequerycontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknmultilinequerycontrol.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)

#### Cách include

```cpp
#include <aknmultilinequerycontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknmultilinequerycontrol.h"
findstr /s /n /i /c:"#include <aknmultilinequerycontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknmultilinequerycontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknmultilinequerycontrol.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknmultilinequerycontrol.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknmultilinequerycontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-061-aknnavi-h-md"></a>
## aknnavi.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/061_aknnavi_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-061-aknnavi-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnavi.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Navigation Pane API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md)

#### Cách include

```cpp
#include <aknnavi.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnavi.h"
findstr /s /n /i /c:"#include <aknnavi.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnavi.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnavi.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnavi.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnavi.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-062-aknnavide-h-md"></a>
## aknnavide.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/062_aknnavide_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-062-aknnavide-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnavide.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Navigation Pane API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md)

#### Cách include

```cpp
#include <aknnavide.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnavide.h"
findstr /s /n /i /c:"#include <aknnavide.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnavide.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnavide.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnavide.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnavide.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-063-aknnavidecoratorobserver-h-md"></a>
## aknnavidecoratorobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/063_aknnavidecoratorobserver_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-063-aknnavidecoratorobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnavidecoratorobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Navigation Pane API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md)

#### Cách include

```cpp
#include <aknnavidecoratorobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnavidecoratorobserver.h"
findstr /s /n /i /c:"#include <aknnavidecoratorobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnavidecoratorobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnavidecoratorobserver.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnavidecoratorobserver.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnavidecoratorobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-064-aknnavilabel-h-md"></a>
## aknnavilabel.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/064_aknnavilabel_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-064-aknnavilabel-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnavilabel.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Navigation Pane API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md)

#### Cách include

```cpp
#include <aknnavilabel.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnavilabel.h"
findstr /s /n /i /c:"#include <aknnavilabel.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnavilabel.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnavilabel.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnavilabel.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnavilabel.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-065-aknnaviobserver-h-md"></a>
## aknnaviobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/065_aknnaviobserver_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-065-aknnaviobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnaviobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Navigation Pane API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md)

#### Cách include

```cpp
#include <aknnaviobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnaviobserver.h"
findstr /s /n /i /c:"#include <aknnaviobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnaviobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnaviobserver.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnaviobserver.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnaviobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-066-aknnotecontrol-h-md"></a>
## aknnotecontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/066_aknnotecontrol_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-066-aknnotecontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnotecontrol.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notes API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-070-notes-api-md)

#### Cách include

```cpp
#include <aknnotecontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnotecontrol.h"
findstr /s /n /i /c:"#include <aknnotecontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnotecontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnotecontrol.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnotecontrol.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnotecontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-067-aknnotedialog-h-md"></a>
## aknnotedialog.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/067_aknnotedialog_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-067-aknnotedialog-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnotedialog.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notes API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-070-notes-api-md)

#### Cách include

```cpp
#include <aknnotedialog.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnotedialog.h"
findstr /s /n /i /c:"#include <aknnotedialog.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnotedialog.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnotedialog.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnotedialog.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnotedialog.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-068-aknnotewrappers-h-md"></a>
## aknnotewrappers.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/068_aknnotewrappers_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-068-aknnotewrappers-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnotewrappers.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notes API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-070-notes-api-md)

#### Cách include

```cpp
#include <aknnotewrappers.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnotewrappers.h"
findstr /s /n /i /c:"#include <aknnotewrappers.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnotewrappers.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnotewrappers.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnotewrappers.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnotewrappers.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-069-aknnotify-h-md"></a>
## aknnotify.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/069_aknnotify_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-069-aknnotify-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnotify.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <aknnotify.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnotify.h"
findstr /s /n /i /c:"#include <aknnotify.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnotify.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnotify.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnotify.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnotify.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-070-aknnotifystd-h-md"></a>
## aknnotifystd.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/070_aknnotifystd_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-070-aknnotifystd-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnotifystd.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Cách include

```cpp
#include <aknnotifystd.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnotifystd.h"
findstr /s /n /i /c:"#include <aknnotifystd.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnotifystd.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnotifystd.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnotifystd.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnotifystd.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-071-aknnumed-h-md"></a>
## aknnumed.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/071_aknnumed_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-071-aknnumed-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnumed.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknnumed.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnumed.h"
findstr /s /n /i /c:"#include <aknnumed.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnumed.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnumed.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnumed.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnumed.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-072-aknnumedwin-h-md"></a>
## aknnumedwin.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/072_aknnumedwin_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-072-aknnumedwin-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnumedwin.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknnumedwin.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnumedwin.h"
findstr /s /n /i /c:"#include <aknnumedwin.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnumedwin.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnumedwin.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnumedwin.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnumedwin.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-073-aknnumseced-h-md"></a>
## aknnumseced.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/073_aknnumseced_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-073-aknnumseced-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknnumseced.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <aknnumseced.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknnumseced.h"
findstr /s /n /i /c:"#include <aknnumseced.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknnumseced.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknnumseced.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknnumseced.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknnumseced.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-074-aknpasswordsettingpage-h-md"></a>
## aknpasswordsettingpage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/074_aknpasswordsettingpage_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-074-aknpasswordsettingpage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknpasswordsettingpage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Setting Pages API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-100-setting-pages-api-md)

#### Cách include

```cpp
#include <aknpasswordsettingpage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknpasswordsettingpage.h"
findstr /s /n /i /c:"#include <aknpasswordsettingpage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknpasswordsettingpage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknpasswordsettingpage.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknpasswordsettingpage.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknpasswordsettingpage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-075-aknpictographdrawerinterface-h-md"></a>
## aknpictographdrawerinterface.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/075_aknpictographdrawerinterface_h.md`
- Consolidated reference: `51-header-reference-001-075.md#src-20-public-header-reference-075-aknpictographdrawerinterface-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `aknpictographdrawerinterface.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Pictograph API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-083-pictograph-api-md)

#### Cách include

```cpp
#include <aknpictographdrawerinterface.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\aknpictographdrawerinterface.h"
findstr /s /n /i /c:"#include <aknpictographdrawerinterface.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"aknpictographdrawerinterface.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: aknpictographdrawerinterface.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <aknpictographdrawerinterface.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22aknpictographdrawerinterface.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
