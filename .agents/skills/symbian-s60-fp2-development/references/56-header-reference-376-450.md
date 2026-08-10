# Public header references 376–450

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-20-public-header-reference-376-hwrmlight-h-md"></a>
## hwrmlight.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/376_hwrmlight_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-376-hwrmlight-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `hwrmlight.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Light API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-062-light-api-md)

#### Cách include

```cpp
#include <hwrmlight.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\hwrmlight.h"
findstr /s /n /i /c:"#include <hwrmlight.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"hwrmlight.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: hwrmlight.h
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
#include <hwrmlight.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22hwrmlight.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-377-hwrmvibra-h-md"></a>
## hwrmvibra.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/377_hwrmvibra_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-377-hwrmvibra-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `hwrmvibra.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Vibra API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-114-vibra-api-md)

#### Cách include

```cpp
#include <hwrmvibra.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\hwrmvibra.h"
findstr /s /n /i /c:"#include <hwrmvibra.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"hwrmvibra.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: hwrmvibra.h
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
#include <hwrmvibra.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22hwrmvibra.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-378-imclient-h-md"></a>
## imclient.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/378_imclient_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-378-imclient-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `imclient.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Instant Messaging API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-045-instant-messaging-api-md)

#### Cách include

```cpp
#include <imclient.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\imclient.h"
findstr /s /n /i /c:"#include <imclient.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"imclient.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: imclient.h
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
#include <imclient.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22imclient.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-379-imconnection-h-md"></a>
## imconnection.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/379_imconnection_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-379-imconnection-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `imconnection.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Instant Messaging API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-045-instant-messaging-api-md)

#### Cách include

```cpp
#include <imconnection.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\imconnection.h"
findstr /s /n /i /c:"#include <imconnection.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"imconnection.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: imconnection.h
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
#include <imconnection.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22imconnection.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-380-imerrors-h-md"></a>
## imerrors.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/380_imerrors_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-380-imerrors-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `imerrors.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Instant Messaging API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-045-instant-messaging-api-md)

#### Cách include

```cpp
#include <imerrors.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\imerrors.h"
findstr /s /n /i /c:"#include <imerrors.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"imerrors.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: imerrors.h
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
#include <imerrors.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22imerrors.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-381-imlauncherplugin-h-md"></a>
## imlauncherplugin.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/381_imlauncherplugin_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-381-imlauncherplugin-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `imlauncherplugin.h`

#### Vai trò

Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [IM Application Launch API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-046-im-application-launch-api-md)

#### Cách include

```cpp
#include <imlauncherplugin.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\imlauncherplugin.h"
findstr /s /n /i /c:"#include <imlauncherplugin.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"imlauncherplugin.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: imlauncherplugin.h
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
#include <imlauncherplugin.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22imlauncherplugin.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-382-jri-h-md"></a>
## jri.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/382_jri_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-382-jri-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `jri.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Cách include

```cpp
#include <jri.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\jri.h"
findstr /s /n /i /c:"#include <jri.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"jri.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: jri.h
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
#include <jri.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22jri.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-383-lbs-h-md"></a>
## lbs.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/383_lbs_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-383-lbs-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbs.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbs.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbs.h"
findstr /s /n /i /c:"#include <lbs.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbs.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbs.h
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
#include <lbs.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbs.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-384-lbsclasstypes-h-md"></a>
## lbsclasstypes.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/384_lbsclasstypes_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-384-lbsclasstypes-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsclasstypes.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsclasstypes.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsclasstypes.h"
findstr /s /n /i /c:"#include <lbsclasstypes.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsclasstypes.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsclasstypes.h
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
#include <lbsclasstypes.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsclasstypes.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-385-lbscommon-h-md"></a>
## lbscommon.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/385_lbscommon_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-385-lbscommon-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbscommon.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbscommon.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbscommon.h"
findstr /s /n /i /c:"#include <lbscommon.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbscommon.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbscommon.h
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
#include <lbscommon.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbscommon.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-386-lbscriteria-h-md"></a>
## lbscriteria.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/386_lbscriteria_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-386-lbscriteria-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbscriteria.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbscriteria.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbscriteria.h"
findstr /s /n /i /c:"#include <lbscriteria.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbscriteria.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbscriteria.h
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
#include <lbscriteria.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbscriteria.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-387-lbserrors-h-md"></a>
## lbserrors.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/387_lbserrors_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-387-lbserrors-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbserrors.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbserrors.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbserrors.h"
findstr /s /n /i /c:"#include <lbserrors.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbserrors.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbserrors.h
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
#include <lbserrors.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbserrors.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-388-lbsfieldids-h-md"></a>
## lbsfieldids.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/388_lbsfieldids_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-388-lbsfieldids-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsfieldids.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsfieldids.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsfieldids.h"
findstr /s /n /i /c:"#include <lbsfieldids.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsfieldids.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsfieldids.h
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
#include <lbsfieldids.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsfieldids.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-389-lbsfields-h-md"></a>
## lbsfields.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/389_lbsfields_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-389-lbsfields-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsfields.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsfields.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsfields.h"
findstr /s /n /i /c:"#include <lbsfields.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsfields.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsfields.h
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
#include <lbsfields.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsfields.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-390-lbsipc-h-md"></a>
## lbsipc.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/390_lbsipc_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-390-lbsipc-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsipc.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsipc.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsipc.h"
findstr /s /n /i /c:"#include <lbsipc.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsipc.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsipc.h
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
#include <lbsipc.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsipc.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-391-lbsposition-h-md"></a>
## lbsposition.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/391_lbsposition_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-391-lbsposition-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsposition.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsposition.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsposition.h"
findstr /s /n /i /c:"#include <lbsposition.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsposition.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsposition.h
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
#include <lbsposition.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsposition.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-392-lbspositioninfo-h-md"></a>
## lbspositioninfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/392_lbspositioninfo_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-392-lbspositioninfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbspositioninfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbspositioninfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbspositioninfo.h"
findstr /s /n /i /c:"#include <lbspositioninfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbspositioninfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbspositioninfo.h
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
#include <lbspositioninfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbspositioninfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-393-lbsptrholder-h-md"></a>
## lbsptrholder.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/393_lbsptrholder_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-393-lbsptrholder-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsptrholder.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsptrholder.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsptrholder.h"
findstr /s /n /i /c:"#include <lbsptrholder.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsptrholder.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsptrholder.h
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
#include <lbsptrholder.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsptrholder.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-394-lbsrequestor-h-md"></a>
## lbsrequestor.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/394_lbsrequestor_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-394-lbsrequestor-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbsrequestor.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbsrequestor.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbsrequestor.h"
findstr /s /n /i /c:"#include <lbsrequestor.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbsrequestor.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbsrequestor.h
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
#include <lbsrequestor.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbsrequestor.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-395-lbssatellite-h-md"></a>
## lbssatellite.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/395_lbssatellite_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-395-lbssatellite-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `lbssatellite.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Cách include

```cpp
#include <lbssatellite.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\lbssatellite.h"
findstr /s /n /i /c:"#include <lbssatellite.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"lbssatellite.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: lbssatellite.h
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
#include <lbssatellite.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22lbssatellite.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-396-listenerdopplerbase-h-md"></a>
## listenerdopplerbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/396_listenerdopplerbase_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-396-listenerdopplerbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `listenerdopplerbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <listenerdopplerbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\listenerdopplerbase.h"
findstr /s /n /i /c:"#include <listenerdopplerbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"listenerdopplerbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: listenerdopplerbase.h
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
#include <listenerdopplerbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22listenerdopplerbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-397-listenerlocationbase-h-md"></a>
## listenerlocationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/397_listenerlocationbase_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-397-listenerlocationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `listenerlocationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <listenerlocationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\listenerlocationbase.h"
findstr /s /n /i /c:"#include <listenerlocationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"listenerlocationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: listenerlocationbase.h
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
#include <listenerlocationbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22listenerlocationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-398-listenerorientationbase-h-md"></a>
## listenerorientationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/398_listenerorientationbase_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-398-listenerorientationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `listenerorientationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <listenerorientationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\listenerorientationbase.h"
findstr /s /n /i /c:"#include <listenerorientationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"listenerorientationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: listenerorientationbase.h
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
#include <listenerorientationbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22listenerorientationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-399-locationbase-h-md"></a>
## locationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/399_locationbase_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-399-locationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `locationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <locationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\locationbase.h"
findstr /s /n /i /c:"#include <locationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"locationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: locationbase.h
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
#include <locationbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22locationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-400-locationdata-h-md"></a>
## locationdata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/400_locationdata_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-400-locationdata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `locationdata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <locationdata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\locationdata.h"
findstr /s /n /i /c:"#include <locationdata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"locationdata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: locationdata.h
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
#include <locationdata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22locationdata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-401-loudnessbase-h-md"></a>
## loudnessbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/401_loudnessbase_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-401-loudnessbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `loudnessbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <loudnessbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\loudnessbase.h"
findstr /s /n /i /c:"#include <loudnessbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"loudnessbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: loudnessbase.h
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
#include <loudnessbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22loudnessbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-402-loudnessdata-h-md"></a>
## loudnessdata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/402_loudnessdata_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-402-loudnessdata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `loudnessdata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <loudnessdata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\loudnessdata.h"
findstr /s /n /i /c:"#include <loudnessdata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"loudnessdata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: loudnessdata.h
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
#include <loudnessdata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22loudnessdata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-403-maknfilefilter-h-md"></a>
## maknfilefilter.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/403_maknfilefilter_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-403-maknfilefilter-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `maknfilefilter.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Common File Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md)

#### Cách include

```cpp
#include <maknfilefilter.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\maknfilefilter.h"
findstr /s /n /i /c:"#include <maknfilefilter.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"maknfilefilter.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: maknfilefilter.h
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
#include <maknfilefilter.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22maknfilefilter.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-404-maknfileselectionobserver-h-md"></a>
## maknfileselectionobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/404_maknfileselectionobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-404-maknfileselectionobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `maknfileselectionobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Common File Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md)

#### Cách include

```cpp
#include <maknfileselectionobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\maknfileselectionobserver.h"
findstr /s /n /i /c:"#include <maknfileselectionobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"maknfileselectionobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: maknfileselectionobserver.h
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
#include <maknfileselectionobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22maknfileselectionobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-405-maknmemoryselectionobserver-h-md"></a>
## maknmemoryselectionobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/405_maknmemoryselectionobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-405-maknmemoryselectionobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `maknmemoryselectionobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Common File Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md)

#### Cách include

```cpp
#include <maknmemoryselectionobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\maknmemoryselectionobserver.h"
findstr /s /n /i /c:"#include <maknmemoryselectionobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"maknmemoryselectionobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: maknmemoryselectionobserver.h
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
#include <maknmemoryselectionobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22maknmemoryselectionobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-406-maudioeffectobserver-h-md"></a>
## maudioeffectobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/406_maudioeffectobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-406-maudioeffectobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `maudioeffectobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <maudioeffectobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\maudioeffectobserver.h"
findstr /s /n /i /c:"#include <maudioeffectobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"maudioeffectobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: maudioeffectobserver.h
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
#include <maudioeffectobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22maudioeffectobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-407-maudioequalizerobserver-h-md"></a>
## maudioequalizerobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/407_maudioequalizerobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-407-maudioequalizerobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `maudioequalizerobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <maudioequalizerobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\maudioequalizerobserver.h"
findstr /s /n /i /c:"#include <maudioequalizerobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"maudioequalizerobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: maudioequalizerobserver.h
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
#include <maudioequalizerobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22maudioequalizerobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-408-mbassboostobserver-h-md"></a>
## mbassboostobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/408_mbassboostobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-408-mbassboostobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mbassboostobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mbassboostobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mbassboostobserver.h"
findstr /s /n /i /c:"#include <mbassboostobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mbassboostobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mbassboostobserver.h
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
#include <mbassboostobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mbassboostobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-409-mclfchangeditemobserver-h-md"></a>
## mclfchangeditemobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/409_mclfchangeditemobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-409-mclfchangeditemobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfchangeditemobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfchangeditemobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfchangeditemobserver.h"
findstr /s /n /i /c:"#include <mclfchangeditemobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfchangeditemobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfchangeditemobserver.h
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
#include <mclfchangeditemobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfchangeditemobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-410-mclfcontentlistingengine-h-md"></a>
## mclfcontentlistingengine.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/410_mclfcontentlistingengine_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-410-mclfcontentlistingengine-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfcontentlistingengine.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfcontentlistingengine.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfcontentlistingengine.h"
findstr /s /n /i /c:"#include <mclfcontentlistingengine.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfcontentlistingengine.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfcontentlistingengine.h
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
#include <mclfcontentlistingengine.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfcontentlistingengine.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-411-mclfcustomgrouper-h-md"></a>
## mclfcustomgrouper.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/411_mclfcustomgrouper_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-411-mclfcustomgrouper-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfcustomgrouper.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfcustomgrouper.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfcustomgrouper.h"
findstr /s /n /i /c:"#include <mclfcustomgrouper.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfcustomgrouper.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfcustomgrouper.h
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
#include <mclfcustomgrouper.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfcustomgrouper.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-412-mclfcustomsorter-h-md"></a>
## mclfcustomsorter.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/412_mclfcustomsorter_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-412-mclfcustomsorter-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfcustomsorter.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfcustomsorter.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfcustomsorter.h"
findstr /s /n /i /c:"#include <mclfcustomsorter.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfcustomsorter.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfcustomsorter.h
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
#include <mclfcustomsorter.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfcustomsorter.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-413-mclfitem-h-md"></a>
## mclfitem.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/413_mclfitem_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-413-mclfitem-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfitem.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfitem.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfitem.h"
findstr /s /n /i /c:"#include <mclfitem.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfitem.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfitem.h
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
#include <mclfitem.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfitem.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-414-mclfitemlistmodel-h-md"></a>
## mclfitemlistmodel.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/414_mclfitemlistmodel_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-414-mclfitemlistmodel-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfitemlistmodel.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfitemlistmodel.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfitemlistmodel.h"
findstr /s /n /i /c:"#include <mclfitemlistmodel.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfitemlistmodel.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfitemlistmodel.h
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
#include <mclfitemlistmodel.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfitemlistmodel.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-415-mclfmodifiableitem-h-md"></a>
## mclfmodifiableitem.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/415_mclfmodifiableitem_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-415-mclfmodifiableitem-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfmodifiableitem.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfmodifiableitem.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfmodifiableitem.h"
findstr /s /n /i /c:"#include <mclfmodifiableitem.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfmodifiableitem.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfmodifiableitem.h
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
#include <mclfmodifiableitem.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfmodifiableitem.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-416-mclfoperationobserver-h-md"></a>
## mclfoperationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/416_mclfoperationobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-416-mclfoperationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfoperationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfoperationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfoperationobserver.h"
findstr /s /n /i /c:"#include <mclfoperationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfoperationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfoperationobserver.h
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
#include <mclfoperationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfoperationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-417-mclfpostfilter-h-md"></a>
## mclfpostfilter.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/417_mclfpostfilter_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-417-mclfpostfilter-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfpostfilter.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfpostfilter.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfpostfilter.h"
findstr /s /n /i /c:"#include <mclfpostfilter.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfpostfilter.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfpostfilter.h
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
#include <mclfpostfilter.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfpostfilter.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-418-mclfprocessobserver-h-md"></a>
## mclfprocessobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/418_mclfprocessobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-418-mclfprocessobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfprocessobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfprocessobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfprocessobserver.h"
findstr /s /n /i /c:"#include <mclfprocessobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfprocessobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfprocessobserver.h
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
#include <mclfprocessobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfprocessobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-419-mclfsortingstyle-h-md"></a>
## mclfsortingstyle.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/419_mclfsortingstyle_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-419-mclfsortingstyle-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mclfsortingstyle.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Cách include

```cpp
#include <mclfsortingstyle.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mclfsortingstyle.h"
findstr /s /n /i /c:"#include <mclfsortingstyle.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mclfsortingstyle.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mclfsortingstyle.h
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
#include <mclfsortingstyle.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mclfsortingstyle.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-420-mdistanceattenuationobserver-h-md"></a>
## mdistanceattenuationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/420_mdistanceattenuationobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-420-mdistanceattenuationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mdistanceattenuationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mdistanceattenuationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mdistanceattenuationobserver.h"
findstr /s /n /i /c:"#include <mdistanceattenuationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mdistanceattenuationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mdistanceattenuationobserver.h
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
#include <mdistanceattenuationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mdistanceattenuationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-421-mdopplerobserver-h-md"></a>
## mdopplerobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/421_mdopplerobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-421-mdopplerobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mdopplerobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mdopplerobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mdopplerobserver.h"
findstr /s /n /i /c:"#include <mdopplerobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mdopplerobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mdopplerobserver.h
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
#include <mdopplerobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mdopplerobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-422-mediafiletypes-hrh-md"></a>
## mediafiletypes.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/422_mediafiletypes_hrh.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-422-mediafiletypes-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mediafiletypes.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Media Fetch API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-066-media-fetch-api-md)

#### Cách include

```cpp
#include <mediafiletypes.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mediafiletypes.hrh"
findstr /s /n /i /c:"#include <mediafiletypes.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mediafiletypes.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mediafiletypes.hrh
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
#include <mediafiletypes.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mediafiletypes.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-423-menvironmentalreverbobserver-h-md"></a>
## menvironmentalreverbobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/423_menvironmentalreverbobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-423-menvironmentalreverbobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `menvironmentalreverbobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <menvironmentalreverbobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\menvironmentalreverbobserver.h"
findstr /s /n /i /c:"#include <menvironmentalreverbobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"menvironmentalreverbobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: menvironmentalreverbobserver.h
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
#include <menvironmentalreverbobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22menvironmentalreverbobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-424-mgfetch-h-md"></a>
## mgfetch.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/424_mgfetch_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-424-mgfetch-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mgfetch.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Media Fetch API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-066-media-fetch-api-md)

#### Cách include

```cpp
#include <mgfetch.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mgfetch.h"
findstr /s /n /i /c:"#include <mgfetch.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mgfetch.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mgfetch.h
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
#include <mgfetch.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mgfetch.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-425-mlistenerdopplerobserver-h-md"></a>
## mlistenerdopplerobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/425_mlistenerdopplerobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-425-mlistenerdopplerobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mlistenerdopplerobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mlistenerdopplerobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mlistenerdopplerobserver.h"
findstr /s /n /i /c:"#include <mlistenerdopplerobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mlistenerdopplerobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mlistenerdopplerobserver.h
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
#include <mlistenerdopplerobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mlistenerdopplerobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-426-mlistenerlocationobserver-h-md"></a>
## mlistenerlocationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/426_mlistenerlocationobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-426-mlistenerlocationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mlistenerlocationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mlistenerlocationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mlistenerlocationobserver.h"
findstr /s /n /i /c:"#include <mlistenerlocationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mlistenerlocationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mlistenerlocationobserver.h
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
#include <mlistenerlocationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mlistenerlocationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-427-mlistenerorientationobserver-h-md"></a>
## mlistenerorientationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/427_mlistenerorientationobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-427-mlistenerorientationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mlistenerorientationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mlistenerorientationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mlistenerorientationobserver.h"
findstr /s /n /i /c:"#include <mlistenerorientationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mlistenerorientationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mlistenerorientationobserver.h
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
#include <mlistenerorientationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mlistenerorientationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-428-mlocationobserver-h-md"></a>
## mlocationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/428_mlocationobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-428-mlocationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mlocationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mlocationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mlocationobserver.h"
findstr /s /n /i /c:"#include <mlocationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mlocationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mlocationobserver.h
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
#include <mlocationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mlocationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-429-mloudnessobserver-h-md"></a>
## mloudnessobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/429_mloudnessobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-429-mloudnessobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mloudnessobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mloudnessobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mloudnessobserver.h"
findstr /s /n /i /c:"#include <mloudnessobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mloudnessobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mloudnessobserver.h
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
#include <mloudnessobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mloudnessobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-430-mmfropcustomcommandconstants-h-md"></a>
## mmfropcustomcommandconstants.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/430_mmfropcustomcommandconstants_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-430-mmfropcustomcommandconstants-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmfropcustomcommandconstants.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [ROP Custom Commands API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-091-rop-custom-commands-api-md)

#### Cách include

```cpp
#include <mmfropcustomcommandconstants.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmfropcustomcommandconstants.h"
findstr /s /n /i /c:"#include <mmfropcustomcommandconstants.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmfropcustomcommandconstants.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmfropcustomcommandconstants.h
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
#include <mmfropcustomcommandconstants.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmfropcustomcommandconstants.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-431-mmgfetchcanceler-h-md"></a>
## mmgfetchcanceler.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/431_mmgfetchcanceler_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-431-mmgfetchcanceler-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmgfetchcanceler.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Media Fetch API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-066-media-fetch-api-md)

#### Cách include

```cpp
#include <mmgfetchcanceler.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmgfetchcanceler.h"
findstr /s /n /i /c:"#include <mmgfetchcanceler.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmgfetchcanceler.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmgfetchcanceler.h
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
#include <mmgfetchcanceler.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmgfetchcanceler.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-432-mmgfetchverifier-h-md"></a>
## mmgfetchverifier.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/432_mmgfetchverifier_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-432-mmgfetchverifier-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmgfetchverifier.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Media Fetch API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-066-media-fetch-api-md)

#### Cách include

```cpp
#include <mmgfetchverifier.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmgfetchverifier.h"
findstr /s /n /i /c:"#include <mmgfetchverifier.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmgfetchverifier.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmgfetchverifier.h
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
#include <mmgfetchverifier.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmgfetchverifier.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-433-mmsclient-h-md"></a>
## mmsclient.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/433_mmsclient_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-433-mmsclient-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmsclient.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [MMS Client MTM API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-065-mms-client-mtm-api-md)

#### Cách include

```cpp
#include <mmsclient.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmsclient.h"
findstr /s /n /i /c:"#include <mmsclient.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmsclient.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmsclient.h
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
#include <mmsclient.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmsclient.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-434-mmsconst-h-md"></a>
## mmsconst.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/434_mmsconst_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-434-mmsconst-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmsconst.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [MMS Client MTM API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-065-mms-client-mtm-api-md)

#### Cách include

```cpp
#include <mmsconst.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmsconst.h"
findstr /s /n /i /c:"#include <mmsconst.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmsconst.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmsconst.h
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
#include <mmsconst.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmsconst.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-435-mmserrors-h-md"></a>
## mmserrors.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/435_mmserrors_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-435-mmserrors-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmserrors.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [MMS Client MTM API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-065-mms-client-mtm-api-md)

#### Cách include

```cpp
#include <mmserrors.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmserrors.h"
findstr /s /n /i /c:"#include <mmserrors.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmserrors.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmserrors.h
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
#include <mmserrors.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmserrors.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-436-mmsgbiocontrol-h-md"></a>
## mmsgbiocontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/436_mmsgbiocontrol_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-436-mmsgbiocontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmsgbiocontrol.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Cách include

```cpp
#include <mmsgbiocontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmsgbiocontrol.h"
findstr /s /n /i /c:"#include <mmsgbiocontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmsgbiocontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmsgbiocontrol.h
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
#include <mmsgbiocontrol.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmsgbiocontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-437-mmsgbiocontrolextension-h-md"></a>
## mmsgbiocontrolextension.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/437_mmsgbiocontrolextension_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-437-mmsgbiocontrolextension-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mmsgbiocontrolextension.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Cách include

```cpp
#include <mmsgbiocontrolextension.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mmsgbiocontrolextension.h"
findstr /s /n /i /c:"#include <mmsgbiocontrolextension.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mmsgbiocontrolextension.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mmsgbiocontrolextension.h
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
#include <mmsgbiocontrolextension.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mmsgbiocontrolextension.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-438-mncnnotification-h-md"></a>
## mncnnotification.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/438_mncnnotification_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-438-mncnnotification-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mncnnotification.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [New Message Notification API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-073-new-message-notification-api-md)

#### Cách include

```cpp
#include <mncnnotification.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mncnnotification.h"
findstr /s /n /i /c:"#include <mncnnotification.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mncnnotification.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mncnnotification.h
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
#include <mncnnotification.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mncnnotification.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-439-morientationobserver-h-md"></a>
## morientationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/439_morientationobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-439-morientationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `morientationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <morientationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\morientationobserver.h"
findstr /s /n /i /c:"#include <morientationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"morientationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: morientationobserver.h
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
#include <morientationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22morientationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-440-mpbkcontactdbobserver-h-md"></a>
## mpbkcontactdbobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/440_mpbkcontactdbobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-440-mpbkcontactdbobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mpbkcontactdbobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <mpbkcontactdbobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mpbkcontactdbobserver.h"
findstr /s /n /i /c:"#include <mpbkcontactdbobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mpbkcontactdbobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mpbkcontactdbobserver.h
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
#include <mpbkcontactdbobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mpbkcontactdbobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-441-mpbkeditoroktoexitcallback-h-md"></a>
## mpbkeditoroktoexitcallback.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/441_mpbkeditoroktoexitcallback_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-441-mpbkeditoroktoexitcallback-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mpbkeditoroktoexitcallback.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Add Edit UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-077-phonebook-data-add-edit-ui-api-md)

#### Cách include

```cpp
#include <mpbkeditoroktoexitcallback.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mpbkeditoroktoexitcallback.h"
findstr /s /n /i /c:"#include <mpbkeditoroktoexitcallback.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mpbkeditoroktoexitcallback.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mpbkeditoroktoexitcallback.h
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
#include <mpbkeditoroktoexitcallback.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mpbkeditoroktoexitcallback.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-442-mpbkfetchcallbacks-h-md"></a>
## mpbkfetchcallbacks.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/442_mpbkfetchcallbacks_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-442-mpbkfetchcallbacks-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mpbkfetchcallbacks.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <mpbkfetchcallbacks.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mpbkfetchcallbacks.h"
findstr /s /n /i /c:"#include <mpbkfetchcallbacks.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mpbkfetchcallbacks.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mpbkfetchcallbacks.h
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
#include <mpbkfetchcallbacks.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mpbkfetchcallbacks.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-443-mpbkfetchdlgselection-h-md"></a>
## mpbkfetchdlgselection.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/443_mpbkfetchdlgselection_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-443-mpbkfetchdlgselection-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mpbkfetchdlgselection.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <mpbkfetchdlgselection.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mpbkfetchdlgselection.h"
findstr /s /n /i /c:"#include <mpbkfetchdlgselection.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mpbkfetchdlgselection.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mpbkfetchdlgselection.h
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
#include <mpbkfetchdlgselection.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mpbkfetchdlgselection.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-444-mpbkfielddata-h-md"></a>
## mpbkfielddata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/444_mpbkfielddata_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-444-mpbkfielddata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mpbkfielddata.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <mpbkfielddata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mpbkfielddata.h"
findstr /s /n /i /c:"#include <mpbkfielddata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mpbkfielddata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mpbkfielddata.h
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
#include <mpbkfielddata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mpbkfielddata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-445-mpbkthumbnailoperationobservers-h-md"></a>
## mpbkthumbnailoperationobservers.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/445_mpbkthumbnailoperationobservers_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-445-mpbkthumbnailoperationobservers-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mpbkthumbnailoperationobservers.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Thumbnail Operation API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-080-phonebook-thumbnail-operation-api-md)

#### Cách include

```cpp
#include <mpbkthumbnailoperationobservers.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mpbkthumbnailoperationobservers.h"
findstr /s /n /i /c:"#include <mpbkthumbnailoperationobservers.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mpbkthumbnailoperationobservers.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mpbkthumbnailoperationobservers.h
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
#include <mpbkthumbnailoperationobservers.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mpbkthumbnailoperationobservers.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-446-mproengactiveprofileobserver-h-md"></a>
## mproengactiveprofileobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/446_mproengactiveprofileobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-446-mproengactiveprofileobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengactiveprofileobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengactiveprofileobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengactiveprofileobserver.h"
findstr /s /n /i /c:"#include <mproengactiveprofileobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengactiveprofileobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengactiveprofileobserver.h
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
#include <mproengactiveprofileobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengactiveprofileobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-447-mproengalerttoneseeker-h-md"></a>
## mproengalerttoneseeker.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/447_mproengalerttoneseeker_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-447-mproengalerttoneseeker-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengalerttoneseeker.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengalerttoneseeker.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengalerttoneseeker.h"
findstr /s /n /i /c:"#include <mproengalerttoneseeker.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengalerttoneseeker.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengalerttoneseeker.h
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
#include <mproengalerttoneseeker.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengalerttoneseeker.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-448-mproengalerttoneseekerobserver-h-md"></a>
## mproengalerttoneseekerobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/448_mproengalerttoneseekerobserver_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-448-mproengalerttoneseekerobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengalerttoneseekerobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengalerttoneseekerobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengalerttoneseekerobserver.h"
findstr /s /n /i /c:"#include <mproengalerttoneseekerobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengalerttoneseekerobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengalerttoneseekerobserver.h
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
#include <mproengalerttoneseekerobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengalerttoneseekerobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-449-mproengengine-h-md"></a>
## mproengengine.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/449_mproengengine_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-449-mproengengine-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengengine.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengengine.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengengine.h"
findstr /s /n /i /c:"#include <mproengengine.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengengine.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengengine.h
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
#include <mproengengine.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengengine.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-450-mproengnotifyhandler-h-md"></a>
## mproengnotifyhandler.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/450_mproengnotifyhandler_h.md`
- Consolidated reference: `56-header-reference-376-450.md#src-20-public-header-reference-450-mproengnotifyhandler-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengnotifyhandler.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengnotifyhandler.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengnotifyhandler.h"
findstr /s /n /i /c:"#include <mproengnotifyhandler.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengnotifyhandler.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengnotifyhandler.h
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
#include <mproengnotifyhandler.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengnotifyhandler.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
