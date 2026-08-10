# Public header references 226–300

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-20-public-header-reference-226-cpbkcontactitem-h-md"></a>
## cpbkcontactitem.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/226_cpbkcontactitem_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-226-cpbkcontactitem-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkcontactitem.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <cpbkcontactitem.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkcontactitem.h"
findstr /s /n /i /c:"#include <cpbkcontactitem.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkcontactitem.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkcontactitem.h
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
#include <cpbkcontactitem.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkcontactitem.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-227-cpbkcontactiter-h-md"></a>
## cpbkcontactiter.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/227_cpbkcontactiter_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-227-cpbkcontactiter-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkcontactiter.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <cpbkcontactiter.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkcontactiter.h"
findstr /s /n /i /c:"#include <cpbkcontactiter.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkcontactiter.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkcontactiter.h
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
#include <cpbkcontactiter.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkcontactiter.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-228-cpbkdatasaveappui-h-md"></a>
## cpbkdatasaveappui.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/228_cpbkdatasaveappui_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-228-cpbkdatasaveappui-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkdatasaveappui.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Add Edit UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-077-phonebook-data-add-edit-ui-api-md)

#### Cách include

```cpp
#include <cpbkdatasaveappui.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkdatasaveappui.h"
findstr /s /n /i /c:"#include <cpbkdatasaveappui.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkdatasaveappui.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkdatasaveappui.h
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
#include <cpbkdatasaveappui.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkdatasaveappui.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-229-cpbkemailaddressselect-h-md"></a>
## cpbkemailaddressselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/229_cpbkemailaddressselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-229-cpbkemailaddressselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkemailaddressselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkemailaddressselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkemailaddressselect.h"
findstr /s /n /i /c:"#include <cpbkemailaddressselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkemailaddressselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkemailaddressselect.h
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
#include <cpbkemailaddressselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkemailaddressselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-230-cpbkemailoversmsaddressselect-h-md"></a>
## cpbkemailoversmsaddressselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/230_cpbkemailoversmsaddressselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-230-cpbkemailoversmsaddressselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkemailoversmsaddressselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkemailoversmsaddressselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkemailoversmsaddressselect.h"
findstr /s /n /i /c:"#include <cpbkemailoversmsaddressselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkemailoversmsaddressselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkemailoversmsaddressselect.h
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
#include <cpbkemailoversmsaddressselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkemailoversmsaddressselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-231-cpbkfieldinfo-h-md"></a>
## cpbkfieldinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/231_cpbkfieldinfo_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-231-cpbkfieldinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkfieldinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <cpbkfieldinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkfieldinfo.h"
findstr /s /n /i /c:"#include <cpbkfieldinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkfieldinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkfieldinfo.h
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
#include <cpbkfieldinfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkfieldinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-232-cpbkfieldsinfo-h-md"></a>
## cpbkfieldsinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/232_cpbkfieldsinfo_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-232-cpbkfieldsinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkfieldsinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <cpbkfieldsinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkfieldsinfo.h"
findstr /s /n /i /c:"#include <cpbkfieldsinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkfieldsinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkfieldsinfo.h
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
#include <cpbkfieldsinfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkfieldsinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-233-cpbkidlefinder-h-md"></a>
## cpbkidlefinder.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/233_cpbkidlefinder_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-233-cpbkidlefinder-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkidlefinder.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <cpbkidlefinder.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkidlefinder.h"
findstr /s /n /i /c:"#include <cpbkidlefinder.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkidlefinder.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkidlefinder.h
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
#include <cpbkidlefinder.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkidlefinder.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-234-cpbkmemoryentryadditemdlg-h-md"></a>
## cpbkmemoryentryadditemdlg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/234_cpbkmemoryentryadditemdlg_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-234-cpbkmemoryentryadditemdlg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkmemoryentryadditemdlg.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkmemoryentryadditemdlg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkmemoryentryadditemdlg.h"
findstr /s /n /i /c:"#include <cpbkmemoryentryadditemdlg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkmemoryentryadditemdlg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkmemoryentryadditemdlg.h
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
#include <cpbkmemoryentryadditemdlg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkmemoryentryadditemdlg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-235-cpbkmmsaddressselect-h-md"></a>
## cpbkmmsaddressselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/235_cpbkmmsaddressselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-235-cpbkmmsaddressselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkmmsaddressselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkmmsaddressselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkmmsaddressselect.h"
findstr /s /n /i /c:"#include <cpbkmmsaddressselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkmmsaddressselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkmmsaddressselect.h
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
#include <cpbkmmsaddressselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkmmsaddressselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-236-cpbkmultipleentryfetchdlg-h-md"></a>
## cpbkmultipleentryfetchdlg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/236_cpbkmultipleentryfetchdlg_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-236-cpbkmultipleentryfetchdlg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkmultipleentryfetchdlg.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkmultipleentryfetchdlg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkmultipleentryfetchdlg.h"
findstr /s /n /i /c:"#include <cpbkmultipleentryfetchdlg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkmultipleentryfetchdlg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkmultipleentryfetchdlg.h
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
#include <cpbkmultipleentryfetchdlg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkmultipleentryfetchdlg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-237-cpbkphonenumberselect-h-md"></a>
## cpbkphonenumberselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/237_cpbkphonenumberselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-237-cpbkphonenumberselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkphonenumberselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkphonenumberselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkphonenumberselect.h"
findstr /s /n /i /c:"#include <cpbkphonenumberselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkphonenumberselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkphonenumberselect.h
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
#include <cpbkphonenumberselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkphonenumberselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-238-cpbkphonenumberselectbase-h-md"></a>
## cpbkphonenumberselectbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/238_cpbkphonenumberselectbase_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-238-cpbkphonenumberselectbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkphonenumberselectbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkphonenumberselectbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkphonenumberselectbase.h"
findstr /s /n /i /c:"#include <cpbkphonenumberselectbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkphonenumberselectbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkphonenumberselectbase.h
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
#include <cpbkphonenumberselectbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkphonenumberselectbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-239-cpbkpocaddressselect-h-md"></a>
## cpbkpocaddressselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/239_cpbkpocaddressselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-239-cpbkpocaddressselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkpocaddressselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkpocaddressselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkpocaddressselect.h"
findstr /s /n /i /c:"#include <cpbkpocaddressselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkpocaddressselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkpocaddressselect.h
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
#include <cpbkpocaddressselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkpocaddressselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-240-cpbkselectfielddlg-h-md"></a>
## cpbkselectfielddlg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/240_cpbkselectfielddlg_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-240-cpbkselectfielddlg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkselectfielddlg.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkselectfielddlg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkselectfielddlg.h"
findstr /s /n /i /c:"#include <cpbkselectfielddlg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkselectfielddlg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkselectfielddlg.h
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
#include <cpbkselectfielddlg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkselectfielddlg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-241-cpbksingleentryfetchdlg-h-md"></a>
## cpbksingleentryfetchdlg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/241_cpbksingleentryfetchdlg_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-241-cpbksingleentryfetchdlg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbksingleentryfetchdlg.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbksingleentryfetchdlg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbksingleentryfetchdlg.h"
findstr /s /n /i /c:"#include <cpbksingleentryfetchdlg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbksingleentryfetchdlg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbksingleentryfetchdlg.h
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
#include <cpbksingleentryfetchdlg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbksingleentryfetchdlg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-242-cpbksingleitemfetchdlg-h-md"></a>
## cpbksingleitemfetchdlg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/242_cpbksingleitemfetchdlg_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-242-cpbksingleitemfetchdlg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbksingleitemfetchdlg.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbksingleitemfetchdlg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbksingleitemfetchdlg.h"
findstr /s /n /i /c:"#include <cpbksingleitemfetchdlg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbksingleitemfetchdlg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbksingleitemfetchdlg.h
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
#include <cpbksingleitemfetchdlg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbksingleitemfetchdlg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-243-cpbksmsaddressselect-h-md"></a>
## cpbksmsaddressselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/243_cpbksmsaddressselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-243-cpbksmsaddressselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbksmsaddressselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbksmsaddressselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbksmsaddressselect.h"
findstr /s /n /i /c:"#include <cpbksmsaddressselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbksmsaddressselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbksmsaddressselect.h
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
#include <cpbksmsaddressselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbksmsaddressselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-244-cpbkvideonumberselect-h-md"></a>
## cpbkvideonumberselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/244_cpbkvideonumberselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-244-cpbkvideonumberselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkvideonumberselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkvideonumberselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkvideonumberselect.h"
findstr /s /n /i /c:"#include <cpbkvideonumberselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkvideonumberselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkvideonumberselect.h
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
#include <cpbkvideonumberselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkvideonumberselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-245-cpbkviewstate-h-md"></a>
## cpbkviewstate.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/245_cpbkviewstate_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-245-cpbkviewstate-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkviewstate.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook UI Utilities API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-081-phonebook-ui-utilities-api-md)

#### Cách include

```cpp
#include <cpbkviewstate.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkviewstate.h"
findstr /s /n /i /c:"#include <cpbkviewstate.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkviewstate.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkviewstate.h
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
#include <cpbkviewstate.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkviewstate.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-246-cpbkvoipaddressselect-h-md"></a>
## cpbkvoipaddressselect.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/246_cpbkvoipaddressselect_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-246-cpbkvoipaddressselect-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `cpbkvoipaddressselect.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Cách include

```cpp
#include <cpbkvoipaddressselect.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\cpbkvoipaddressselect.h"
findstr /s /n /i /c:"#include <cpbkvoipaddressselect.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"cpbkvoipaddressselect.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: cpbkvoipaddressselect.h
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
#include <cpbkvoipaddressselect.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22cpbkvoipaddressselect.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-247-crichbio-h-md"></a>
## crichbio.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/247_crichbio_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-247-crichbio-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `crichbio.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Cách include

```cpp
#include <crichbio.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\crichbio.h"
findstr /s /n /i /c:"#include <crichbio.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"crichbio.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: crichbio.h
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
#include <crichbio.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22crichbio.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-248-csatelliteinfoui-h-md"></a>
## csatelliteinfoui.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/248_csatelliteinfoui_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-248-csatelliteinfoui-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `csatelliteinfoui.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [BLID Application Satellite Info API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-010-blid-application-satellite-info-api-md)

#### Cách include

```cpp
#include <csatelliteinfoui.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\csatelliteinfoui.h"
findstr /s /n /i /c:"#include <csatelliteinfoui.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"csatelliteinfoui.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: csatelliteinfoui.h
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
#include <csatelliteinfoui.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22csatelliteinfoui.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-249-csendingserviceinfo-h-md"></a>
## csendingserviceinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/249_csendingserviceinfo_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-249-csendingserviceinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `csendingserviceinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Send UI API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md)

#### Cách include

```cpp
#include <csendingserviceinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\csendingserviceinfo.h"
findstr /s /n /i /c:"#include <csendingserviceinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"csendingserviceinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: csendingserviceinfo.h
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
#include <csendingserviceinfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22csendingserviceinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-250-distanceattenuationbase-h-md"></a>
## distanceattenuationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/250_distanceattenuationbase_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-250-distanceattenuationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `distanceattenuationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <distanceattenuationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\distanceattenuationbase.h"
findstr /s /n /i /c:"#include <distanceattenuationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"distanceattenuationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: distanceattenuationbase.h
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
#include <distanceattenuationbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22distanceattenuationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-251-distanceattenuationdata-h-md"></a>
## distanceattenuationdata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/251_distanceattenuationdata_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-251-distanceattenuationdata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `distanceattenuationdata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <distanceattenuationdata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\distanceattenuationdata.h"
findstr /s /n /i /c:"#include <distanceattenuationdata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"distanceattenuationdata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: distanceattenuationdata.h
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
#include <distanceattenuationdata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22distanceattenuationdata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-252-documenthandler-h-md"></a>
## documenthandler.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/252_documenthandler_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-252-documenthandler-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `documenthandler.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Document Handler API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-032-document-handler-api-md)

#### Cách include

```cpp
#include <documenthandler.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\documenthandler.h"
findstr /s /n /i /c:"#include <documenthandler.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"documenthandler.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: documenthandler.h
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
#include <documenthandler.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22documenthandler.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-253-dopplerbase-h-md"></a>
## dopplerbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/253_dopplerbase_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-253-dopplerbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `dopplerbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <dopplerbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\dopplerbase.h"
findstr /s /n /i /c:"#include <dopplerbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"dopplerbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: dopplerbase.h
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
#include <dopplerbase.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22dopplerbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-254-dopplerdata-h-md"></a>
## dopplerdata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/254_dopplerdata_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-254-dopplerdata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `dopplerdata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <dopplerdata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\dopplerdata.h"
findstr /s /n /i /c:"#include <dopplerdata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"dopplerdata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: dopplerdata.h
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
#include <dopplerdata.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22dopplerdata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-255-downloadmgrclient-h-md"></a>
## downloadmgrclient.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/255_downloadmgrclient_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-255-downloadmgrclient-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `downloadmgrclient.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Download Manager Engine API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-033-download-manager-engine-api-md)

#### Cách include

```cpp
#include <downloadmgrclient.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\downloadmgrclient.h"
findstr /s /n /i /c:"#include <downloadmgrclient.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"downloadmgrclient.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: downloadmgrclient.h
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
#include <downloadmgrclient.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22downloadmgrclient.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-256-downloadmgrclientapiext-h-md"></a>
## downloadmgrclientapiext.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/256_downloadmgrclientapiext_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-256-downloadmgrclientapiext-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `downloadmgrclientapiext.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Download Manager Engine API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-033-download-manager-engine-api-md)

#### Cách include

```cpp
#include <downloadmgrclientapiext.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\downloadmgrclientapiext.h"
findstr /s /n /i /c:"#include <downloadmgrclientapiext.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"downloadmgrclientapiext.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: downloadmgrclientapiext.h
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
#include <downloadmgrclientapiext.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22downloadmgrclientapiext.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-257-downloadslistdlgobserver-h-md"></a>
## downloadslistdlgobserver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/257_downloadslistdlgobserver_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-257-downloadslistdlgobserver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `downloadslistdlgobserver.h`

#### Vai trò

Observer/callback interface; implement it and keep the observer alive through asynchronous notifications.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Download Manager UI Library API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-034-download-manager-ui-library-api-md)

#### Cách include

```cpp
#include <downloadslistdlgobserver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\downloadslistdlgobserver.h"
findstr /s /n /i /c:"#include <downloadslistdlgobserver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"downloadslistdlgobserver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: downloadslistdlgobserver.h
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
#include <downloadslistdlgobserver.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22downloadslistdlgobserver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-258-drmaudiosampleplayer-h-md"></a>
## drmaudiosampleplayer.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/258_drmaudiosampleplayer_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-258-drmaudiosampleplayer-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `drmaudiosampleplayer.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [DRM Audio Player API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-026-drm-audio-player-api-md)

#### Cách include

```cpp
#include <drmaudiosampleplayer.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\drmaudiosampleplayer.h"
findstr /s /n /i /c:"#include <drmaudiosampleplayer.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"drmaudiosampleplayer.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: drmaudiosampleplayer.h
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
#include <drmaudiosampleplayer.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22drmaudiosampleplayer.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-259-drmhelper-h-md"></a>
## drmhelper.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/259_drmhelper_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-259-drmhelper-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `drmhelper.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [DRM Helper API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-027-drm-helper-api-md)

#### Cách include

```cpp
#include <drmhelper.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\drmhelper.h"
findstr /s /n /i /c:"#include <drmhelper.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"drmhelper.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: drmhelper.h
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
#include <drmhelper.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22drmhelper.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-260-drmhelperserverinternalcrkeys-h-md"></a>
## drmhelperserverinternalcrkeys.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/260_drmhelperserverinternalcrkeys_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-260-drmhelperserverinternalcrkeys-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `drmhelperserverinternalcrkeys.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [DRM Helper API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-027-drm-helper-api-md)

#### Cách include

```cpp
#include <drmhelperserverinternalcrkeys.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\drmhelperserverinternalcrkeys.h"
findstr /s /n /i /c:"#include <drmhelperserverinternalcrkeys.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"drmhelperserverinternalcrkeys.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: drmhelperserverinternalcrkeys.h
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
#include <drmhelperserverinternalcrkeys.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22drmhelperserverinternalcrkeys.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-261-drmlicensechecker-h-md"></a>
## drmlicensechecker.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/261_drmlicensechecker_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-261-drmlicensechecker-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `drmlicensechecker.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [DRM License Checker API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-028-drm-license-checker-api-md)

#### Cách include

```cpp
#include <drmlicensechecker.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\drmlicensechecker.h"
findstr /s /n /i /c:"#include <drmlicensechecker.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"drmlicensechecker.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: drmlicensechecker.h
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
#include <drmlicensechecker.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22drmlicensechecker.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-262-eikalign-h-md"></a>
## eikalign.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/262_eikalign_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-262-eikalign-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikalign.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Base Controls API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-011-base-controls-api-md)

#### Cách include

```cpp
#include <eikalign.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikalign.h"
findstr /s /n /i /c:"#include <eikalign.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikalign.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikalign.h
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
#include <eikalign.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikalign.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-263-eikbctrl-h-md"></a>
## eikbctrl.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/263_eikbctrl_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-263-eikbctrl-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikbctrl.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Base Controls API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-011-base-controls-api-md)

#### Cách include

```cpp
#include <eikbctrl.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikbctrl.h"
findstr /s /n /i /c:"#include <eikbctrl.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikbctrl.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikbctrl.h
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
#include <eikbctrl.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikbctrl.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-264-eikbgfty-h-md"></a>
## eikbgfty.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/264_eikbgfty_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-264-eikbgfty-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikbgfty.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikbgfty.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikbgfty.h"
findstr /s /n /i /c:"#include <eikbgfty.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikbgfty.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikbgfty.h
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
#include <eikbgfty.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikbgfty.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-265-eikbtgpc-h-md"></a>
## eikbtgpc.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/265_eikbtgpc_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-265-eikbtgpc-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikbtgpc.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikbtgpc.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikbtgpc.h"
findstr /s /n /i /c:"#include <eikbtgpc.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikbtgpc.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikbtgpc.h
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
#include <eikbtgpc.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikbtgpc.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-266-eikbtgps-h-md"></a>
## eikbtgps.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/266_eikbtgps_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-266-eikbtgps-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikbtgps.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikbtgps.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikbtgps.h"
findstr /s /n /i /c:"#include <eikbtgps.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikbtgps.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikbtgps.h
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
#include <eikbtgps.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikbtgps.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-267-eikbtgrp-h-md"></a>
## eikbtgrp.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/267_eikbtgrp_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-267-eikbtgrp-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikbtgrp.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikbtgrp.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikbtgrp.h"
findstr /s /n /i /c:"#include <eikbtgrp.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikbtgrp.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikbtgrp.h
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
#include <eikbtgrp.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikbtgrp.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-268-eikbutb-h-md"></a>
## eikbutb.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/268_eikbutb_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-268-eikbutb-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikbutb.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikbutb.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikbutb.h"
findstr /s /n /i /c:"#include <eikbutb.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikbutb.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikbutb.h
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
#include <eikbutb.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikbutb.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-269-eikcal-h-md"></a>
## eikcal.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/269_eikcal_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-269-eikcal-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikcal.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Calendar API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-015-calendar-api-md)

#### Cách include

```cpp
#include <eikcal.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikcal.h"
findstr /s /n /i /c:"#include <eikcal.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikcal.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikcal.h
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
#include <eikcal.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikcal.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-270-eikcapc-h-md"></a>
## eikcapc.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/270_eikcapc_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-270-eikcapc-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikcapc.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Misc Controls API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md)

#### Cách include

```cpp
#include <eikcapc.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikcapc.h"
findstr /s /n /i /c:"#include <eikcapc.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikcapc.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikcapc.h
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
#include <eikcapc.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikcapc.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-271-eikcapca-h-md"></a>
## eikcapca.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/271_eikcapca_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-271-eikcapca-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikcapca.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Misc Controls API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md)

#### Cách include

```cpp
#include <eikcapca.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikcapca.h"
findstr /s /n /i /c:"#include <eikcapca.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikcapca.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikcapca.h
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
#include <eikcapca.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikcapca.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-272-eikcba-h-md"></a>
## eikcba.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/272_eikcba_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-272-eikcba-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikcba.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikcba.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikcba.h"
findstr /s /n /i /c:"#include <eikcba.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikcba.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikcba.h
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
#include <eikcba.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikcba.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-273-eikccpu-h-md"></a>
## eikccpu.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/273_eikccpu_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-273-eikccpu-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikccpu.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <eikccpu.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikccpu.h"
findstr /s /n /i /c:"#include <eikccpu.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikccpu.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikccpu.h
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
#include <eikccpu.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikccpu.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-274-eikclb-h-md"></a>
## eikclb.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/274_eikclb_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-274-eikclb-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikclb.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eikclb.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikclb.h"
findstr /s /n /i /c:"#include <eikclb.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikclb.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikclb.h
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
#include <eikclb.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikclb.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-275-eikclbd-h-md"></a>
## eikclbd.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/275_eikclbd_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-275-eikclbd-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikclbd.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eikclbd.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikclbd.h"
findstr /s /n /i /c:"#include <eikclbd.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikclbd.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikclbd.h
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
#include <eikclbd.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikclbd.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-276-eikcmbut-h-md"></a>
## eikcmbut.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/276_eikcmbut_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-276-eikcmbut-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikcmbut.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eikcmbut.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikcmbut.h"
findstr /s /n /i /c:"#include <eikcmbut.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikcmbut.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikcmbut.h
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
#include <eikcmbut.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikcmbut.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-277-eikcolib-h-md"></a>
## eikcolib.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/277_eikcolib_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-277-eikcolib-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikcolib.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Initialization API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-051-initialization-api-md)

#### Cách include

```cpp
#include <eikcolib.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikcolib.h"
findstr /s /n /i /c:"#include <eikcolib.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikcolib.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikcolib.h
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
#include <eikcolib.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikcolib.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-278-eikconso-h-md"></a>
## eikconso.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/278_eikconso_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-278-eikconso-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikconso.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Misc Controls API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md)

#### Cách include

```cpp
#include <eikconso.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikconso.h"
findstr /s /n /i /c:"#include <eikconso.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikconso.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikconso.h
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
#include <eikconso.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikconso.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-279-eikctgrp-h-md"></a>
## eikctgrp.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/279_eikctgrp_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-279-eikctgrp-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikctgrp.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Misc Controls API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md)

#### Cách include

```cpp
#include <eikctgrp.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikctgrp.h"
findstr /s /n /i /c:"#include <eikctgrp.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikctgrp.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikctgrp.h
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
#include <eikctgrp.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikctgrp.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-280-eikctlib-h-md"></a>
## eikctlib.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/280_eikctlib_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-280-eikctlib-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikctlib.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Initialization API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-051-initialization-api-md)

#### Cách include

```cpp
#include <eikctlib.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikctlib.h"
findstr /s /n /i /c:"#include <eikctlib.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikctlib.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikctlib.h
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
#include <eikctlib.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikctlib.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-281-eikdialg-h-md"></a>
## eikdialg.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/281_eikdialg_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-281-eikdialg-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikdialg.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-030-dialogs-api-md)

#### Cách include

```cpp
#include <eikdialg.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikdialg.h"
findstr /s /n /i /c:"#include <eikdialg.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikdialg.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikdialg.h
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
#include <eikdialg.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikdialg.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-282-eikdpobs-h-md"></a>
## eikdpobs.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/282_eikdpobs_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-282-eikdpobs-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikdpobs.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Misc Controls API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md)

#### Cách include

```cpp
#include <eikdpobs.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikdpobs.h"
findstr /s /n /i /c:"#include <eikdpobs.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikdpobs.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikdpobs.h
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
#include <eikdpobs.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikdpobs.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-283-eikedwin-h-md"></a>
## eikedwin.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/283_eikedwin_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-283-eikedwin-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikedwin.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <eikedwin.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikedwin.h"
findstr /s /n /i /c:"#include <eikedwin.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikedwin.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikedwin.h
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
#include <eikedwin.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikedwin.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-284-eikedwob-h-md"></a>
## eikedwob.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/284_eikedwob_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-284-eikedwob-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikedwob.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <eikedwob.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikedwob.h"
findstr /s /n /i /c:"#include <eikedwob.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikedwob.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikedwob.h
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
#include <eikedwob.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikedwob.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-285-eikfnlab-h-md"></a>
## eikfnlab.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/285_eikfnlab_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-285-eikfnlab-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikfnlab.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Labels API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-056-labels-api-md)

#### Cách include

```cpp
#include <eikfnlab.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikfnlab.h"
findstr /s /n /i /c:"#include <eikfnlab.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikfnlab.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikfnlab.h
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
#include <eikfnlab.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikfnlab.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-286-eikfpne-h-md"></a>
## eikfpne.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/286_eikfpne_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-286-eikfpne-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikfpne.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <eikfpne.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikfpne.h"
findstr /s /n /i /c:"#include <eikfpne.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikfpne.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikfpne.h
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
#include <eikfpne.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikfpne.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-287-eikfrlb-h-md"></a>
## eikfrlb.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/287_eikfrlb_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-287-eikfrlb-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikfrlb.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eikfrlb.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikfrlb.h"
findstr /s /n /i /c:"#include <eikfrlb.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikfrlb.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikfrlb.h
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
#include <eikfrlb.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikfrlb.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-288-eikfrlbd-h-md"></a>
## eikfrlbd.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/288_eikfrlbd_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-288-eikfrlbd-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikfrlbd.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eikfrlbd.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikfrlbd.h"
findstr /s /n /i /c:"#include <eikfrlbd.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikfrlbd.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikfrlbd.h
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
#include <eikfrlbd.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikfrlbd.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-289-eikgted-h-md"></a>
## eikgted.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/289_eikgted_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-289-eikgted-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikgted.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Editors API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md)

#### Cách include

```cpp
#include <eikgted.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikgted.h"
findstr /s /n /i /c:"#include <eikgted.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikgted.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikgted.h
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
#include <eikgted.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikgted.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-290-eikhkeyt-h-md"></a>
## eikhkeyt.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/290_eikhkeyt_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-290-eikhkeyt-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikhkeyt.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Hotkeys API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-044-hotkeys-api-md)

#### Cách include

```cpp
#include <eikhkeyt.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikhkeyt.h"
findstr /s /n /i /c:"#include <eikhkeyt.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikhkeyt.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikhkeyt.h
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
#include <eikhkeyt.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikhkeyt.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-291-eikimage-h-md"></a>
## eikimage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/291_eikimage_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-291-eikimage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikimage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Images API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-047-images-api-md)

#### Cách include

```cpp
#include <eikimage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikimage.h"
findstr /s /n /i /c:"#include <eikimage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikimage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikimage.h
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
#include <eikimage.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikimage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-292-eikinfo-h-md"></a>
## eikinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/292_eikinfo_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-292-eikinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Notes API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-070-notes-api-md)

#### Cách include

```cpp
#include <eikinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikinfo.h"
findstr /s /n /i /c:"#include <eikinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikinfo.h
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
#include <eikinfo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-293-eikkwin-h-md"></a>
## eikkwin.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/293_eikkwin_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-293-eikkwin-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eikkwin.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Misc Controls API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md)

#### Cách include

```cpp
#include <eikkwin.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eikkwin.h"
findstr /s /n /i /c:"#include <eikkwin.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eikkwin.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eikkwin.h
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
#include <eikkwin.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eikkwin.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-294-eiklabel-h-md"></a>
## eiklabel.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/294_eiklabel_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-294-eiklabel-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklabel.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Labels API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-056-labels-api-md)

#### Cách include

```cpp
#include <eiklabel.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklabel.h"
findstr /s /n /i /c:"#include <eiklabel.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklabel.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklabel.h
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
#include <eiklabel.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklabel.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-295-eiklbbut-h-md"></a>
## eiklbbut.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/295_eiklbbut_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-295-eiklbbut-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklbbut.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Cách include

```cpp
#include <eiklbbut.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklbbut.h"
findstr /s /n /i /c:"#include <eiklbbut.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklbbut.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklbbut.h
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
#include <eiklbbut.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklbbut.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-296-eiklbd-h-md"></a>
## eiklbd.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/296_eiklbd_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-296-eiklbd-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklbd.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eiklbd.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklbd.h"
findstr /s /n /i /c:"#include <eiklbd.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklbd.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklbd.h
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
#include <eiklbd.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklbd.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-297-eiklbed-h-md"></a>
## eiklbed.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/297_eiklbed_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-297-eiklbed-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklbed.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eiklbed.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklbed.h"
findstr /s /n /i /c:"#include <eiklbed.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklbed.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklbed.h
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
#include <eiklbed.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklbed.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-298-eiklbi-h-md"></a>
## eiklbi.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/298_eiklbi_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-298-eiklbi-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklbi.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eiklbi.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklbi.h"
findstr /s /n /i /c:"#include <eiklbi.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklbi.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklbi.h
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
#include <eiklbi.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklbi.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-299-eiklbm-h-md"></a>
## eiklbm.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/299_eiklbm_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-299-eiklbm-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklbm.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eiklbm.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklbm.h"
findstr /s /n /i /c:"#include <eiklbm.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklbm.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklbm.h
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
#include <eiklbm.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklbm.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-300-eiklbo-h-md"></a>
## eiklbo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/300_eiklbo_h.md`
- Consolidated reference: `54-header-reference-226-300.md#src-20-public-header-reference-300-eiklbo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `eiklbo.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)

#### Cách include

```cpp
#include <eiklbo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\eiklbo.h"
findstr /s /n /i /c:"#include <eiklbo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"eiklbo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: eiklbo.h
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
#include <eiklbo.h>

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
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22eiklbo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
