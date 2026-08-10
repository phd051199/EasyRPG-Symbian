# Public header references 451–525

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-20-public-header-reference-451-mproengprofile-h-md"></a>
## mproengprofile.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/451_mproengprofile_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-451-mproengprofile-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengprofile.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengprofile.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengprofile.h"
findstr /s /n /i /c:"#include <mproengprofile.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengprofile.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengprofile.h
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
#include <mproengprofile.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengprofile.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-452-mproengprofileactivationobserver-h-md"></a>
## mproengprofileactivationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/452_mproengprofileactivationobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-452-mproengprofileactivationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengprofileactivationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengprofileactivationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengprofileactivationobserver.h"
findstr /s /n /i /c:"#include <mproengprofileactivationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengprofileactivationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengprofileactivationobserver.h
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
#include <mproengprofileactivationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengprofileactivationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-453-mproengprofilename-h-md"></a>
## mproengprofilename.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/453_mproengprofilename_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-453-mproengprofilename-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengprofilename.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengprofilename.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengprofilename.h"
findstr /s /n /i /c:"#include <mproengprofilename.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengprofilename.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengprofilename.h
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
#include <mproengprofilename.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengprofilename.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-454-mproengprofilenamearray-h-md"></a>
## mproengprofilenamearray.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/454_mproengprofilenamearray_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-454-mproengprofilenamearray-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengprofilenamearray.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengprofilenamearray.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengprofilenamearray.h"
findstr /s /n /i /c:"#include <mproengprofilenamearray.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengprofilenamearray.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengprofilenamearray.h
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
#include <mproengprofilenamearray.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengprofilenamearray.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-455-mproengprofilenamearrayobserver-h-md"></a>
## mproengprofilenamearrayobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/455_mproengprofilenamearrayobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-455-mproengprofilenamearrayobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengprofilenamearrayobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengprofilenamearrayobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengprofilenamearrayobserver.h"
findstr /s /n /i /c:"#include <mproengprofilenamearrayobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengprofilenamearrayobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengprofilenamearrayobserver.h
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
#include <mproengprofilenamearrayobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengprofilenamearrayobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-456-mproengprofileobserver-h-md"></a>
## mproengprofileobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/456_mproengprofileobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-456-mproengprofileobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengprofileobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengprofileobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengprofileobserver.h"
findstr /s /n /i /c:"#include <mproengprofileobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengprofileobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengprofileobserver.h
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
#include <mproengprofileobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengprofileobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-457-mproengtones-h-md"></a>
## mproengtones.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/457_mproengtones_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-457-mproengtones-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengtones.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengtones.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengtones.h"
findstr /s /n /i /c:"#include <mproengtones.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengtones.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengtones.h
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
#include <mproengtones.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengtones.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-458-mproengtonesettings-h-md"></a>
## mproengtonesettings.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/458_mproengtonesettings_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-458-mproengtonesettings-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mproengtonesettings.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <mproengtonesettings.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mproengtonesettings.h"
findstr /s /n /i /c:"#include <mproengtonesettings.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mproengtonesettings.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mproengtonesettings.h
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
#include <mproengtonesettings.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mproengtonesettings.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-459-mroomlevelobserver-h-md"></a>
## mroomlevelobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/459_mroomlevelobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-459-mroomlevelobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mroomlevelobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mroomlevelobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mroomlevelobserver.h"
findstr /s /n /i /c:"#include <mroomlevelobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mroomlevelobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mroomlevelobserver.h
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
#include <mroomlevelobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mroomlevelobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-460-msenauthenticationprovider-h-md"></a>
## msenauthenticationprovider.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/460_msenauthenticationprovider_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-460-msenauthenticationprovider-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenauthenticationprovider.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <msenauthenticationprovider.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenauthenticationprovider.h"
findstr /s /n /i /c:"#include <msenauthenticationprovider.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenauthenticationprovider.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenauthenticationprovider.h
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
#include <msenauthenticationprovider.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenauthenticationprovider.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-461-msenconsumerpolicy-h-md"></a>
## msenconsumerpolicy.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/461_msenconsumerpolicy_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-461-msenconsumerpolicy-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenconsumerpolicy.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <msenconsumerpolicy.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenconsumerpolicy.h"
findstr /s /n /i /c:"#include <msenconsumerpolicy.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenconsumerpolicy.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenconsumerpolicy.h
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
#include <msenconsumerpolicy.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenconsumerpolicy.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-462-msencontenthandlerclient-h-md"></a>
## msencontenthandlerclient.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/462_msencontenthandlerclient_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-462-msencontenthandlerclient-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msencontenthandlerclient.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <msencontenthandlerclient.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msencontenthandlerclient.h"
findstr /s /n /i /c:"#include <msencontenthandlerclient.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msencontenthandlerclient.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msencontenthandlerclient.h
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
#include <msencontenthandlerclient.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msencontenthandlerclient.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-463-msenelement-h-md"></a>
## msenelement.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/463_msenelement_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-463-msenelement-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenelement.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <msenelement.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenelement.h"
findstr /s /n /i /c:"#include <msenelement.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenelement.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenelement.h
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
#include <msenelement.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenelement.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-464-msenfragment-h-md"></a>
## msenfragment.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/464_msenfragment_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-464-msenfragment-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenfragment.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <msenfragment.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenfragment.h"
findstr /s /n /i /c:"#include <msenfragment.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenfragment.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenfragment.h
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
#include <msenfragment.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenfragment.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-465-msenidentityprovideridarray-h-md"></a>
## msenidentityprovideridarray.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/465_msenidentityprovideridarray_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-465-msenidentityprovideridarray-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenidentityprovideridarray.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <msenidentityprovideridarray.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenidentityprovideridarray.h"
findstr /s /n /i /c:"#include <msenidentityprovideridarray.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenidentityprovideridarray.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenidentityprovideridarray.h
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
#include <msenidentityprovideridarray.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenidentityprovideridarray.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-466-msenproperties-h-md"></a>
## msenproperties.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/466_msenproperties_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-466-msenproperties-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenproperties.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <msenproperties.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenproperties.h"
findstr /s /n /i /c:"#include <msenproperties.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenproperties.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenproperties.h
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
#include <msenproperties.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenproperties.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-467-msenproperty-h-md"></a>
## msenproperty.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/467_msenproperty_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-467-msenproperty-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenproperty.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <msenproperty.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenproperty.h"
findstr /s /n /i /c:"#include <msenproperty.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenproperty.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenproperty.h
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
#include <msenproperty.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenproperty.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-468-msenproviderpolicy-h-md"></a>
## msenproviderpolicy.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/468_msenproviderpolicy_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-468-msenproviderpolicy-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenproviderpolicy.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <msenproviderpolicy.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenproviderpolicy.h"
findstr /s /n /i /c:"#include <msenproviderpolicy.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenproviderpolicy.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenproviderpolicy.h
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
#include <msenproviderpolicy.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenproviderpolicy.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-469-msenserviceconsumer-h-md"></a>
## msenserviceconsumer.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/469_msenserviceconsumer_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-469-msenserviceconsumer-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenserviceconsumer.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <msenserviceconsumer.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenserviceconsumer.h"
findstr /s /n /i /c:"#include <msenserviceconsumer.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenserviceconsumer.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenserviceconsumer.h
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
#include <msenserviceconsumer.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenserviceconsumer.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-470-msenservicedescription-h-md"></a>
## msenservicedescription.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/470_msenservicedescription_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-470-msenservicedescription-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenservicedescription.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <msenservicedescription.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenservicedescription.h"
findstr /s /n /i /c:"#include <msenservicedescription.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenservicedescription.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenservicedescription.h
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
#include <msenservicedescription.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenservicedescription.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-471-msenservicepolicy-h-md"></a>
## msenservicepolicy.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/471_msenservicepolicy_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-471-msenservicepolicy-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msenservicepolicy.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <msenservicepolicy.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msenservicepolicy.h"
findstr /s /n /i /c:"#include <msenservicepolicy.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msenservicepolicy.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msenservicepolicy.h
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
#include <msenservicepolicy.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msenservicepolicy.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-472-msgbiocontrol-h-md"></a>
## msgbiocontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/472_msgbiocontrol_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-472-msgbiocontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msgbiocontrol.h`

#### Vai trò

Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Cách include

```cpp
#include <msgbiocontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msgbiocontrol.h"
findstr /s /n /i /c:"#include <msgbiocontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msgbiocontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msgbiocontrol.h
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
#include <msgbiocontrol.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msgbiocontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-473-msgbiocontrolobserver-h-md"></a>
## msgbiocontrolobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/473_msgbiocontrolobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-473-msgbiocontrolobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msgbiocontrolobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Cách include

```cpp
#include <msgbiocontrolobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msgbiocontrolobserver.h"
findstr /s /n /i /c:"#include <msgbiocontrolobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msgbiocontrolobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msgbiocontrolobserver.h
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
#include <msgbiocontrolobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msgbiocontrolobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-474-msgbiouids-h-md"></a>
## msgbiouids.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/474_msgbiouids_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-474-msgbiouids-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msgbiouids.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Cách include

```cpp
#include <msgbiouids.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msgbiouids.h"
findstr /s /n /i /c:"#include <msgbiouids.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msgbiouids.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msgbiouids.h
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
#include <msgbiouids.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msgbiouids.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-475-msourcedopplerobserver-h-md"></a>
## msourcedopplerobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/475_msourcedopplerobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-475-msourcedopplerobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msourcedopplerobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <msourcedopplerobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msourcedopplerobserver.h"
findstr /s /n /i /c:"#include <msourcedopplerobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msourcedopplerobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msourcedopplerobserver.h
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
#include <msourcedopplerobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msourcedopplerobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-476-msourcelocationobserver-h-md"></a>
## msourcelocationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/476_msourcelocationobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-476-msourcelocationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msourcelocationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <msourcelocationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msourcelocationobserver.h"
findstr /s /n /i /c:"#include <msourcelocationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msourcelocationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msourcelocationobserver.h
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
#include <msourcelocationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msourcelocationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-477-msourceorientationobserver-h-md"></a>
## msourceorientationobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/477_msourceorientationobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-477-msourceorientationobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `msourceorientationobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <msourceorientationobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\msourceorientationobserver.h"
findstr /s /n /i /c:"#include <msourceorientationobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"msourceorientationobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: msourceorientationobserver.h
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
#include <msourceorientationobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22msourceorientationobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-478-mstereowideningobserver-h-md"></a>
## mstereowideningobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/478_mstereowideningobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-478-mstereowideningobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `mstereowideningobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <mstereowideningobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\mstereowideningobserver.h"
findstr /s /n /i /c:"#include <mstereowideningobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"mstereowideningobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: mstereowideningobserver.h
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
#include <mstereowideningobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22mstereowideningobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-479-npapi-h-md"></a>
## npapi.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/479_npapi_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-479-npapi-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `npapi.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Cách include

```cpp
#include <npapi.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\npapi.h"
findstr /s /n /i /c:"#include <npapi.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"npapi.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: npapi.h
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
#include <npapi.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22npapi.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-480-npdapi-h-md"></a>
## npdapi.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/480_npdapi_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-480-npdapi-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `npdapi.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notepad Library API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-069-notepad-library-api-md)

#### Cách include

```cpp
#include <npdapi.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\npdapi.h"
findstr /s /n /i /c:"#include <npdapi.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"npdapi.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: npdapi.h
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
#include <npdapi.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22npdapi.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-481-npupp-h-md"></a>
## npupp.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/481_npupp_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-481-npupp-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `npupp.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Cách include

```cpp
#include <npupp.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\npupp.h"
findstr /s /n /i /c:"#include <npupp.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"npupp.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: npupp.h
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
#include <npupp.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22npupp.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-482-ocrcommon-h-md"></a>
## ocrcommon.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/482_ocrcommon_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-482-ocrcommon-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ocrcommon.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Optical Character Recognition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-074-optical-character-recognition-api-md)

#### Cách include

```cpp
#include <ocrcommon.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ocrcommon.h"
findstr /s /n /i /c:"#include <ocrcommon.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ocrcommon.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ocrcommon.h
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
#include <ocrcommon.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ocrcommon.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-483-ocrsrv-h-md"></a>
## ocrsrv.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/483_ocrsrv_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-483-ocrsrv-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ocrsrv.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Optical Character Recognition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-074-optical-character-recognition-api-md)

#### Cách include

```cpp
#include <ocrsrv.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ocrsrv.h"
findstr /s /n /i /c:"#include <ocrsrv.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ocrsrv.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ocrsrv.h
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
#include <ocrsrv.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ocrsrv.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-484-oma2agent-h-md"></a>
## oma2agent.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/484_oma2agent_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-484-oma2agent-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `oma2agent.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [OMA DRM CAF Agent API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-072-oma-drm-caf-agent-api-md)

#### Cách include

```cpp
#include <oma2agent.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\oma2agent.h"
findstr /s /n /i /c:"#include <oma2agent.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"oma2agent.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: oma2agent.h
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
#include <oma2agent.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22oma2agent.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-485-orientationbase-h-md"></a>
## orientationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/485_orientationbase_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-485-orientationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `orientationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <orientationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\orientationbase.h"
findstr /s /n /i /c:"#include <orientationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"orientationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: orientationbase.h
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
#include <orientationbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22orientationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-486-orientationdata-h-md"></a>
## orientationdata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/486_orientationdata_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-486-orientationdata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `orientationdata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <orientationdata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\orientationdata.h"
findstr /s /n /i /c:"#include <orientationdata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"orientationdata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: orientationdata.h
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
#include <orientationdata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22orientationdata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-487-pathconfiguration-hrh-md"></a>
## pathconfiguration.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/487_pathconfiguration_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-487-pathconfiguration-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pathconfiguration.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Platform Environment API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-084-platform-environment-api-md)

#### Cách include

```cpp
#include <pathconfiguration.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pathconfiguration.hrh"
findstr /s /n /i /c:"#include <pathconfiguration.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pathconfiguration.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pathconfiguration.hrh
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
#include <pathconfiguration.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pathconfiguration.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-488-pathinfo-h-md"></a>
## pathinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/488_pathinfo_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-488-pathinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pathinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Platform Environment API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-084-platform-environment-api-md)

#### Cách include

```cpp
#include <pathinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pathinfo.h"
findstr /s /n /i /c:"#include <pathinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pathinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pathinfo.h
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
#include <pathinfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pathinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-489-pbkdatasaveappui-hrh-md"></a>
## pbkdatasaveappui.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/489_pbkdatasaveappui_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-489-pbkdatasaveappui-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pbkdatasaveappui.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Phonebook Data Add Edit UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-077-phonebook-data-add-edit-ui-api-md)

#### Cách include

```cpp
#include <pbkdatasaveappui.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pbkdatasaveappui.hrh"
findstr /s /n /i /c:"#include <pbkdatasaveappui.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pbkdatasaveappui.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pbkdatasaveappui.hrh
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
#include <pbkdatasaveappui.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pbkdatasaveappui.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-490-pbkfields-hrh-md"></a>
## pbkfields.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/490_pbkfields_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-490-pbkfields-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pbkfields.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <pbkfields.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pbkfields.hrh"
findstr /s /n /i /c:"#include <pbkfields.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pbkfields.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pbkfields.hrh
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
#include <pbkfields.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pbkfields.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-491-pbkiconid-hrh-md"></a>
## pbkiconid.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/491_pbkiconid_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-491-pbkiconid-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pbkiconid.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <pbkiconid.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pbkiconid.hrh"
findstr /s /n /i /c:"#include <pbkiconid.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pbkiconid.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pbkiconid.hrh
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
#include <pbkiconid.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pbkiconid.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-492-pbkiconinfo-h-md"></a>
## pbkiconinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/492_pbkiconinfo_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-492-pbkiconinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pbkiconinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook UI Utilities API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-081-phonebook-ui-utilities-api-md)

#### Cách include

```cpp
#include <pbkiconinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pbkiconinfo.h"
findstr /s /n /i /c:"#include <pbkiconinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pbkiconinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pbkiconinfo.h
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
#include <pbkiconinfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pbkiconinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-493-pluginadapterinterface-h-md"></a>
## pluginadapterinterface.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/493_pluginadapterinterface_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-493-pluginadapterinterface-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pluginadapterinterface.h`

#### Vai trò

Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Cách include

```cpp
#include <pluginadapterinterface.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pluginadapterinterface.h"
findstr /s /n /i /c:"#include <pluginadapterinterface.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pluginadapterinterface.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pluginadapterinterface.h
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
#include <pluginadapterinterface.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pluginadapterinterface.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-494-prcpucfg-h-md"></a>
## prcpucfg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/494_prcpucfg_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-494-prcpucfg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `prcpucfg.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Cách include

```cpp
#include <prcpucfg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\prcpucfg.h"
findstr /s /n /i /c:"#include <prcpucfg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"prcpucfg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: prcpucfg.h
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
#include <prcpucfg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22prcpucfg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-495-proengfactory-h-md"></a>
## proengfactory.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/495_proengfactory_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-495-proengfactory-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `proengfactory.h`

#### Vai trò

Factory entry point; normally creates an interface or implementation selected at runtime.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <proengfactory.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\proengfactory.h"
findstr /s /n /i /c:"#include <proengfactory.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"proengfactory.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: proengfactory.h
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
#include <proengfactory.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22proengfactory.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-496-profile-hrh-md"></a>
## profile.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/496_profile_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-496-profile-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `profile.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)

#### Cách include

```cpp
#include <profile.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\profile.hrh"
findstr /s /n /i /c:"#include <profile.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"profile.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: profile.hrh
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
#include <profile.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22profile.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-497-profileenginesdkcrkeys-h-md"></a>
## profileenginesdkcrkeys.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/497_profileenginesdkcrkeys_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-497-profileenginesdkcrkeys-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `profileenginesdkcrkeys.h`

#### Vai trò

Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Profiles Engine Active Profile Settings API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-087-profiles-engine-active-profile-settings-api-md)

#### Cách include

```cpp
#include <profileenginesdkcrkeys.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\profileenginesdkcrkeys.h"
findstr /s /n /i /c:"#include <profileenginesdkcrkeys.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"profileenginesdkcrkeys.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: profileenginesdkcrkeys.h
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
#include <profileenginesdkcrkeys.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22profileenginesdkcrkeys.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-498-prtypes-h-md"></a>
## prtypes.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/498_prtypes_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-498-prtypes-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `prtypes.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Cách include

```cpp
#include <prtypes.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\prtypes.h"
findstr /s /n /i /c:"#include <prtypes.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"prtypes.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: prtypes.h
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
#include <prtypes.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22prtypes.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-499-pticompositiondataif-h-md"></a>
## pticompositiondataif.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/499_pticompositiondataif_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-499-pticompositiondataif-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `pticompositiondataif.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <pticompositiondataif.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\pticompositiondataif.h"
findstr /s /n /i /c:"#include <pticompositiondataif.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"pticompositiondataif.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: pticompositiondataif.h
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
#include <pticompositiondataif.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22pticompositiondataif.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-500-ptidefs-h-md"></a>
## ptidefs.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/500_ptidefs_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-500-ptidefs-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptidefs.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptidefs.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptidefs.h"
findstr /s /n /i /c:"#include <ptidefs.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptidefs.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptidefs.h
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
#include <ptidefs.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptidefs.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-501-ptiengine-h-md"></a>
## ptiengine.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/501_ptiengine_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-501-ptiengine-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptiengine.h`

#### Vai trò

Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptiengine.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptiengine.h"
findstr /s /n /i /c:"#include <ptiengine.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptiengine.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptiengine.h
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
#include <ptiengine.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptiengine.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-502-ptikeymappings-h-md"></a>
## ptikeymappings.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/502_ptikeymappings_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-502-ptikeymappings-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptikeymappings.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptikeymappings.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptikeymappings.h"
findstr /s /n /i /c:"#include <ptikeymappings.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptikeymappings.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptikeymappings.h
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
#include <ptikeymappings.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptikeymappings.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-503-ptilanguage-h-md"></a>
## ptilanguage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/503_ptilanguage_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-503-ptilanguage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptilanguage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptilanguage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptilanguage.h"
findstr /s /n /i /c:"#include <ptilanguage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptilanguage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptilanguage.h
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
#include <ptilanguage.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptilanguage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-504-ptilanguagedatabase-h-md"></a>
## ptilanguagedatabase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/504_ptilanguagedatabase_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-504-ptilanguagedatabase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptilanguagedatabase.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptilanguagedatabase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptilanguagedatabase.h"
findstr /s /n /i /c:"#include <ptilanguagedatabase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptilanguagedatabase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptilanguagedatabase.h
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
#include <ptilanguagedatabase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptilanguagedatabase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-505-ptiobserver-h-md"></a>
## ptiobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/505_ptiobserver_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-505-ptiobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptiobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptiobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptiobserver.h"
findstr /s /n /i /c:"#include <ptiobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptiobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptiobserver.h
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
#include <ptiobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptiobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-506-ptiuids-hrh-md"></a>
## ptiuids.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/506_ptiuids_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-506-ptiuids-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptiuids.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptiuids.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptiuids.hrh"
findstr /s /n /i /c:"#include <ptiuids.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptiuids.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptiuids.hrh
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
#include <ptiuids.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptiuids.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-507-ptiuserdicentry-h-md"></a>
## ptiuserdicentry.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/507_ptiuserdicentry_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-507-ptiuserdicentry-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptiuserdicentry.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptiuserdicentry.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptiuserdicentry.h"
findstr /s /n /i /c:"#include <ptiuserdicentry.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptiuserdicentry.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptiuserdicentry.h
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
#include <ptiuserdicentry.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptiuserdicentry.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-508-ptiuserdictionary-h-md"></a>
## ptiuserdictionary.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/508_ptiuserdictionary_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-508-ptiuserdictionary-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `ptiuserdictionary.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)

#### Cách include

```cpp
#include <ptiuserdictionary.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\ptiuserdictionary.h"
findstr /s /n /i /c:"#include <ptiuserdictionary.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"ptiuserdictionary.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: ptiuserdictionary.h
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
#include <ptiuserdictionary.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ptiuserdictionary.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-509-rconnmon-h-md"></a>
## rconnmon.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/509_rconnmon_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-509-rconnmon-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `rconnmon.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Connection Monitor Server API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-021-connection-monitor-server-api-md)

#### Cách include

```cpp
#include <rconnmon.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\rconnmon.h"
findstr /s /n /i /c:"#include <rconnmon.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"rconnmon.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: rconnmon.h
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
#include <rconnmon.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22rconnmon.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-510-rdrmhelper-h-md"></a>
## rdrmhelper.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/510_rdrmhelper_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-510-rdrmhelper-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `rdrmhelper.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [DRM Helper API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-027-drm-helper-api-md)

#### Cách include

```cpp
#include <rdrmhelper.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\rdrmhelper.h"
findstr /s /n /i /c:"#include <rdrmhelper.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"rdrmhelper.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: rdrmhelper.h
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
#include <rdrmhelper.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22rdrmhelper.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-511-roomlevelbase-h-md"></a>
## roomlevelbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/511_roomlevelbase_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-511-roomlevelbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `roomlevelbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <roomlevelbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\roomlevelbase.h"
findstr /s /n /i /c:"#include <roomlevelbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"roomlevelbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: roomlevelbase.h
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
#include <roomlevelbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22roomlevelbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-512-roomleveldata-h-md"></a>
## roomleveldata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/512_roomleveldata_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-512-roomleveldata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `roomleveldata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <roomleveldata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\roomleveldata.h"
findstr /s /n /i /c:"#include <roomleveldata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"roomleveldata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: roomleveldata.h
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
#include <roomleveldata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22roomleveldata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-513-rpbkviewresourcefile-h-md"></a>
## rpbkviewresourcefile.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/513_rpbkviewresourcefile_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-513-rpbkviewresourcefile-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `rpbkviewresourcefile.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook UI Utilities API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-081-phonebook-ui-utilities-api-md)

#### Cách include

```cpp
#include <rpbkviewresourcefile.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\rpbkviewresourcefile.h"
findstr /s /n /i /c:"#include <rpbkviewresourcefile.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"rpbkviewresourcefile.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: rpbkviewresourcefile.h
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
#include <rpbkviewresourcefile.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22rpbkviewresourcefile.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-514-screensaverplugin-h-md"></a>
## screensaverplugin.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/514_screensaverplugin_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-514-screensaverplugin-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `screensaverplugin.h`

#### Vai trò

Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Screen Saver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-095-screen-saver-api-md)

#### Cách include

```cpp
#include <screensaverplugin.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\screensaverplugin.h"
findstr /s /n /i /c:"#include <screensaverplugin.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"screensaverplugin.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: screensaverplugin.h
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
#include <screensaverplugin.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22screensaverplugin.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-515-screensaverpluginintdef-h-md"></a>
## screensaverpluginintdef.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/515_screensaverpluginintdef_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-515-screensaverpluginintdef-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `screensaverpluginintdef.h`

#### Vai trò

Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Screen Saver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-095-screen-saver-api-md)

#### Cách include

```cpp
#include <screensaverpluginintdef.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\screensaverpluginintdef.h"
findstr /s /n /i /c:"#include <screensaverpluginintdef.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"screensaverpluginintdef.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: screensaverpluginintdef.h
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
#include <screensaverpluginintdef.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22screensaverpluginintdef.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-516-screensaverpluginintdef-hrh-md"></a>
## screensaverpluginintdef.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/516_screensaverpluginintdef_hrh.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-516-screensaverpluginintdef-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `screensaverpluginintdef.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Screen Saver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-095-screen-saver-api-md)

#### Cách include

```cpp
#include <screensaverpluginintdef.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\screensaverpluginintdef.hrh"
findstr /s /n /i /c:"#include <screensaverpluginintdef.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"screensaverpluginintdef.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: screensaverpluginintdef.hrh
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
#include <screensaverpluginintdef.hrh>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22screensaverpluginintdef.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-517-senbaseattribute-h-md"></a>
## senbaseattribute.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/517_senbaseattribute_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-517-senbaseattribute-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senbaseattribute.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senbaseattribute.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senbaseattribute.h"
findstr /s /n /i /c:"#include <senbaseattribute.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senbaseattribute.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senbaseattribute.h
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
#include <senbaseattribute.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senbaseattribute.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-518-senbaseelement-h-md"></a>
## senbaseelement.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/518_senbaseelement_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-518-senbaseelement-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senbaseelement.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senbaseelement.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senbaseelement.h"
findstr /s /n /i /c:"#include <senbaseelement.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senbaseelement.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senbaseelement.h
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
#include <senbaseelement.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senbaseelement.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-519-senbasefragment-h-md"></a>
## senbasefragment.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/519_senbasefragment_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-519-senbasefragment-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senbasefragment.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senbasefragment.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senbasefragment.h"
findstr /s /n /i /c:"#include <senbasefragment.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senbasefragment.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senbasefragment.h
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
#include <senbasefragment.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senbasefragment.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-520-sencredential-h-md"></a>
## sencredential.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/520_sencredential_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-520-sencredential-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sencredential.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <sencredential.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sencredential.h"
findstr /s /n /i /c:"#include <sencredential.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sencredential.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sencredential.h
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
#include <sencredential.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sencredential.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-521-sencredential2-h-md"></a>
## sencredential2.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/521_sencredential2_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-521-sencredential2-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sencredential2.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <sencredential2.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sencredential2.h"
findstr /s /n /i /c:"#include <sencredential2.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sencredential2.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sencredential2.h
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
#include <sencredential2.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sencredential2.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-522-sendateutils-h-md"></a>
## sendateutils.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/522_sendateutils_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-522-sendateutils-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sendateutils.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <sendateutils.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sendateutils.h"
findstr /s /n /i /c:"#include <sendateutils.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sendateutils.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sendateutils.h
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
#include <sendateutils.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sendateutils.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-523-sendomfragment-h-md"></a>
## sendomfragment.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/523_sendomfragment_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-523-sendomfragment-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sendomfragment.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <sendomfragment.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sendomfragment.h"
findstr /s /n /i /c:"#include <sendomfragment.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sendomfragment.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sendomfragment.h
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
#include <sendomfragment.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sendomfragment.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-524-sendui-h-md"></a>
## sendui.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/524_sendui_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-524-sendui-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sendui.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Send UI API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md)

#### Cách include

```cpp
#include <sendui.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sendui.h"
findstr /s /n /i /c:"#include <sendui.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sendui.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sendui.h
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
#include <sendui.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sendui.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-525-senduiconsts-h-md"></a>
## senduiconsts.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/525_senduiconsts_h.md`
- Consolidated reference: `57-header-reference-451-525.md#src-20-public-header-reference-525-senduiconsts-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senduiconsts.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Send UI API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md)

#### Cách include

```cpp
#include <senduiconsts.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senduiconsts.h"
findstr /s /n /i /c:"#include <senduiconsts.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senduiconsts.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senduiconsts.h
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
#include <senduiconsts.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senduiconsts.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
