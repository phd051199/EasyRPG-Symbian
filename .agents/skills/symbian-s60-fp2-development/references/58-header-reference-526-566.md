# Public header references 526–566

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-20-public-header-reference-526-senduimtmuids-h-md"></a>
## senduimtmuids.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/526_senduimtmuids_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-526-senduimtmuids-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senduimtmuids.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Send UI API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md)

#### Cách include

```cpp
#include <senduimtmuids.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senduimtmuids.h"
findstr /s /n /i /c:"#include <senduimtmuids.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senduimtmuids.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senduimtmuids.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senduimtmuids.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senduimtmuids.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-527-senelement-h-md"></a>
## senelement.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/527_senelement_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-527-senelement-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senelement.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senelement.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senelement.h"
findstr /s /n /i /c:"#include <senelement.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senelement.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senelement.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senelement.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senelement.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-528-senfacet-h-md"></a>
## senfacet.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/528_senfacet_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-528-senfacet-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senfacet.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <senfacet.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senfacet.h"
findstr /s /n /i /c:"#include <senfacet.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senfacet.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senfacet.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senfacet.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senfacet.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-529-senfragment-h-md"></a>
## senfragment.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/529_senfragment_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-529-senfragment-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senfragment.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senfragment.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senfragment.h"
findstr /s /n /i /c:"#include <senfragment.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senfragment.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senfragment.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senfragment.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senfragment.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-530-senhttptransportproperties-h-md"></a>
## senhttptransportproperties.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/530_senhttptransportproperties_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-530-senhttptransportproperties-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senhttptransportproperties.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <senhttptransportproperties.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senhttptransportproperties.h"
findstr /s /n /i /c:"#include <senhttptransportproperties.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senhttptransportproperties.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senhttptransportproperties.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senhttptransportproperties.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senhttptransportproperties.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-531-senidentityprovider-h-md"></a>
## senidentityprovider.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/531_senidentityprovider_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-531-senidentityprovider-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senidentityprovider.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <senidentityprovider.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senidentityprovider.h"
findstr /s /n /i /c:"#include <senidentityprovider.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senidentityprovider.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senidentityprovider.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senidentityprovider.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senidentityprovider.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-532-senidentityprovideridarray8-h-md"></a>
## senidentityprovideridarray8.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/532_senidentityprovideridarray8_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-532-senidentityprovideridarray8-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senidentityprovideridarray8.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <senidentityprovideridarray8.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senidentityprovideridarray8.h"
findstr /s /n /i /c:"#include <senidentityprovideridarray8.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senidentityprovideridarray8.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senidentityprovideridarray8.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senidentityprovideridarray8.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senidentityprovideridarray8.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-533-sennamespace-h-md"></a>
## sennamespace.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/533_sennamespace_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-533-sennamespace-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sennamespace.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <sennamespace.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sennamespace.h"
findstr /s /n /i /c:"#include <sennamespace.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sennamespace.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sennamespace.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sennamespace.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sennamespace.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-534-senserviceconnection-h-md"></a>
## senserviceconnection.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/534_senserviceconnection_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-534-senserviceconnection-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senserviceconnection.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <senserviceconnection.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senserviceconnection.h"
findstr /s /n /i /c:"#include <senserviceconnection.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senserviceconnection.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senserviceconnection.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senserviceconnection.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senserviceconnection.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-535-senservicemanager-h-md"></a>
## senservicemanager.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/535_senservicemanager_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-535-senservicemanager-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senservicemanager.h`

#### Vai trò

Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Manager API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-119-web-service-manager-api-md)

#### Cách include

```cpp
#include <senservicemanager.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senservicemanager.h"
findstr /s /n /i /c:"#include <senservicemanager.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senservicemanager.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senservicemanager.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senservicemanager.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senservicemanager.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-536-senservicepattern-h-md"></a>
## senservicepattern.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/536_senservicepattern_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-536-senservicepattern-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senservicepattern.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <senservicepattern.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senservicepattern.h"
findstr /s /n /i /c:"#include <senservicepattern.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senservicepattern.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senservicepattern.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senservicepattern.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senservicepattern.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-537-sensoapconstants-h-md"></a>
## sensoapconstants.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/537_sensoapconstants_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-537-sensoapconstants-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sensoapconstants.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <sensoapconstants.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sensoapconstants.h"
findstr /s /n /i /c:"#include <sensoapconstants.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sensoapconstants.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sensoapconstants.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sensoapconstants.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sensoapconstants.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-538-sensoapenvelope-h-md"></a>
## sensoapenvelope.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/538_sensoapenvelope_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-538-sensoapenvelope-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sensoapenvelope.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <sensoapenvelope.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sensoapenvelope.h"
findstr /s /n /i /c:"#include <sensoapenvelope.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sensoapenvelope.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sensoapenvelope.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sensoapenvelope.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sensoapenvelope.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-539-sensoapfault-h-md"></a>
## sensoapfault.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/539_sensoapfault_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-539-sensoapfault-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sensoapfault.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <sensoapfault.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sensoapfault.h"
findstr /s /n /i /c:"#include <sensoapfault.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sensoapfault.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sensoapfault.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sensoapfault.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sensoapfault.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-540-sensoapmessage-h-md"></a>
## sensoapmessage.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/540_sensoapmessage_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-540-sensoapmessage-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sensoapmessage.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <sensoapmessage.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sensoapmessage.h"
findstr /s /n /i /c:"#include <sensoapmessage.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sensoapmessage.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sensoapmessage.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sensoapmessage.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sensoapmessage.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-541-sentransportproperties-h-md"></a>
## sentransportproperties.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/541_sentransportproperties_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-541-sentransportproperties-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sentransportproperties.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <sentransportproperties.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sentransportproperties.h"
findstr /s /n /i /c:"#include <sentransportproperties.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sentransportproperties.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sentransportproperties.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sentransportproperties.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sentransportproperties.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-542-senwssecurityheader-h-md"></a>
## senwssecurityheader.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/542_senwssecurityheader_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-542-senwssecurityheader-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senwssecurityheader.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <senwssecurityheader.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senwssecurityheader.h"
findstr /s /n /i /c:"#include <senwssecurityheader.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senwssecurityheader.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senwssecurityheader.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senwssecurityheader.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senwssecurityheader.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-543-senxmlconstants-h-md"></a>
## senxmlconstants.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/543_senxmlconstants_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-543-senxmlconstants-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senxmlconstants.h`

#### Vai trò

Shared constants, enums, UIDs, error codes or common value types.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senxmlconstants.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senxmlconstants.h"
findstr /s /n /i /c:"#include <senxmlconstants.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senxmlconstants.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senxmlconstants.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senxmlconstants.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senxmlconstants.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-544-senxmlelement-h-md"></a>
## senxmlelement.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/544_senxmlelement_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-544-senxmlelement-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senxmlelement.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senxmlelement.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senxmlelement.h"
findstr /s /n /i /c:"#include <senxmlelement.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senxmlelement.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senxmlelement.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senxmlelement.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senxmlelement.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-545-senxmlproperties-h-md"></a>
## senxmlproperties.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/545_senxmlproperties_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-545-senxmlproperties-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senxmlproperties.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)

#### Cách include

```cpp
#include <senxmlproperties.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senxmlproperties.h"
findstr /s /n /i /c:"#include <senxmlproperties.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senxmlproperties.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senxmlproperties.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senxmlproperties.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senxmlproperties.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-546-senxmlreader-h-md"></a>
## senxmlreader.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/546_senxmlreader_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-546-senxmlreader-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senxmlreader.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senxmlreader.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senxmlreader.h"
findstr /s /n /i /c:"#include <senxmlreader.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senxmlreader.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senxmlreader.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senxmlreader.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senxmlreader.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-547-senxmlservicedescription-h-md"></a>
## senxmlservicedescription.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/547_senxmlservicedescription_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-547-senxmlservicedescription-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senxmlservicedescription.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)

#### Cách include

```cpp
#include <senxmlservicedescription.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senxmlservicedescription.h"
findstr /s /n /i /c:"#include <senxmlservicedescription.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senxmlservicedescription.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senxmlservicedescription.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senxmlservicedescription.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senxmlservicedescription.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-548-senxmlutils-h-md"></a>
## senxmlutils.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/548_senxmlutils_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-548-senxmlutils-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `senxmlutils.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Cách include

```cpp
#include <senxmlutils.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\senxmlutils.h"
findstr /s /n /i /c:"#include <senxmlutils.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"senxmlutils.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: senxmlutils.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <senxmlutils.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22senxmlutils.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-549-sourcedopplerbase-h-md"></a>
## sourcedopplerbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/549_sourcedopplerbase_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-549-sourcedopplerbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sourcedopplerbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <sourcedopplerbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sourcedopplerbase.h"
findstr /s /n /i /c:"#include <sourcedopplerbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sourcedopplerbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sourcedopplerbase.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sourcedopplerbase.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sourcedopplerbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-550-sourcelocationbase-h-md"></a>
## sourcelocationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/550_sourcelocationbase_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-550-sourcelocationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sourcelocationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <sourcelocationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sourcelocationbase.h"
findstr /s /n /i /c:"#include <sourcelocationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sourcelocationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sourcelocationbase.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sourcelocationbase.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sourcelocationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-551-sourceorientationbase-h-md"></a>
## sourceorientationbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/551_sourceorientationbase_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-551-sourceorientationbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sourceorientationbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <sourceorientationbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sourceorientationbase.h"
findstr /s /n /i /c:"#include <sourceorientationbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sourceorientationbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sourceorientationbase.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sourceorientationbase.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sourceorientationbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-552-spdiacontrol-h-md"></a>
## spdiacontrol.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/552_spdiacontrol_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-552-spdiacontrol-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `spdiacontrol.h`

#### Vai trò

UI facade/control/dialog declaration; must run on the UI thread with a CONE environment.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Speed Dial Utility Control API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-104-speed-dial-utility-control-api-md)

#### Cách include

```cpp
#include <spdiacontrol.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\spdiacontrol.h"
findstr /s /n /i /c:"#include <spdiacontrol.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"spdiacontrol.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: spdiacontrol.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <spdiacontrol.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22spdiacontrol.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-553-startupitem-hrh-md"></a>
## startupitem.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/553_startupitem_hrh.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-553-startupitem-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `startupitem.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Startup List Management API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-105-startup-list-management-api-md)

#### Cách include

```cpp
#include <startupitem.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\startupitem.hrh"
findstr /s /n /i /c:"#include <startupitem.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"startupitem.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: startupitem.hrh
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <startupitem.hrh>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22startupitem.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-554-stereowideningbase-h-md"></a>
## stereowideningbase.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/554_stereowideningbase_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-554-stereowideningbase-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `stereowideningbase.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <stereowideningbase.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\stereowideningbase.h"
findstr /s /n /i /c:"#include <stereowideningbase.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"stereowideningbase.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: stereowideningbase.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <stereowideningbase.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22stereowideningbase.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-555-stereowideningdata-h-md"></a>
## stereowideningdata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/555_stereowideningdata_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-555-stereowideningdata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `stereowideningdata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Cách include

```cpp
#include <stereowideningdata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\stereowideningdata.h"
findstr /s /n /i /c:"#include <stereowideningdata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"stereowideningdata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: stereowideningdata.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <stereowideningdata.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22stereowideningdata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-556-stereowideningutility-h-md"></a>
## stereowideningutility.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/556_stereowideningutility_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-556-stereowideningutility-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `stereowideningutility.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects Presets API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-009-audio-effects-presets-api-md)

#### Cách include

```cpp
#include <stereowideningutility.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\stereowideningutility.h"
findstr /s /n /i /c:"#include <stereowideningutility.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"stereowideningutility.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: stereowideningutility.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <stereowideningutility.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22stereowideningutility.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-557-stereowideningutilitydata-h-md"></a>
## stereowideningutilitydata.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/557_stereowideningutilitydata_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-557-stereowideningutilitydata-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `stereowideningutilitydata.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Audio Effects Presets API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-009-audio-effects-presets-api-md)

#### Cách include

```cpp
#include <stereowideningutilitydata.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\stereowideningutilitydata.h"
findstr /s /n /i /c:"#include <stereowideningutilitydata.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"stereowideningutilitydata.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: stereowideningutilitydata.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <stereowideningutilitydata.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22stereowideningutilitydata.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-558-stringloader-h-md"></a>
## stringloader.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/558_stringloader_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-558-stringloader-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `stringloader.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [String Loader API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-107-string-loader-api-md)

#### Cách include

```cpp
#include <stringloader.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\stringloader.h"
findstr /s /n /i /c:"#include <stringloader.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"stringloader.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: stringloader.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <stringloader.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22stringloader.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-559-sysutil-h-md"></a>
## sysutil.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/559_sysutil_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-559-sysutil-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `sysutil.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [System Utils API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-108-system-utils-api-md)

#### Cách include

```cpp
#include <sysutil.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\sysutil.h"
findstr /s /n /i /c:"#include <sysutil.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"sysutil.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: sysutil.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <sysutil.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22sysutil.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-560-textresolver-h-md"></a>
## textresolver.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/560_textresolver_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-560-textresolver-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `textresolver.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Text Resolver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-110-text-resolver-api-md)

#### Cách include

```cpp
#include <textresolver.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\textresolver.h"
findstr /s /n /i /c:"#include <textresolver.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"textresolver.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: textresolver.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <textresolver.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22textresolver.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-561-textresolver-hrh-md"></a>
## textresolver.hrh

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/561_textresolver_hrh.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-561-textresolver-hrh-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `textresolver.hrh`

#### Vai trò

Resource/command/enum constants for `.rss`, `.hrh` and C++ integration.

- Loại: **resource header**
- Được Quick Reference liệt kê trong: [Text Resolver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-110-text-resolver-api-md)

#### Cách include

```cpp
#include <textresolver.hrh>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\textresolver.hrh"
findstr /s /n /i /c:"#include <textresolver.hrh>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"textresolver.hrh" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: textresolver.hrh
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <textresolver.hrh>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22textresolver.hrh%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-562-tlmkitemiddbcombiinfo-h-md"></a>
## tlmkitemiddbcombiinfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/562_tlmkitemiddbcombiinfo_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-562-tlmkitemiddbcombiinfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `tlmkitemiddbcombiinfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Landmarks UI Selector API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-061-landmarks-ui-selector-api-md)

#### Cách include

```cpp
#include <tlmkitemiddbcombiinfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\tlmkitemiddbcombiinfo.h"
findstr /s /n /i /c:"#include <tlmkitemiddbcombiinfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"tlmkitemiddbcombiinfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: tlmkitemiddbcombiinfo.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <tlmkitemiddbcombiinfo.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22tlmkitemiddbcombiinfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-563-tpbkcontactitemfield-h-md"></a>
## tpbkcontactitemfield.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/563_tpbkcontactitemfield_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-563-tpbkcontactitemfield-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `tpbkcontactitemfield.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Cách include

```cpp
#include <tpbkcontactitemfield.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\tpbkcontactitemfield.h"
findstr /s /n /i /c:"#include <tpbkcontactitemfield.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"tpbkcontactitemfield.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: tpbkcontactitemfield.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <tpbkcontactitemfield.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22tpbkcontactitemfield.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-564-tsendingcapabilities-h-md"></a>
## tsendingcapabilities.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/564_tsendingcapabilities_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-564-tsendingcapabilities-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `tsendingcapabilities.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Send UI API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md)

#### Cách include

```cpp
#include <tsendingcapabilities.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\tsendingcapabilities.h"
findstr /s /n /i /c:"#include <tsendingcapabilities.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"tsendingcapabilities.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: tsendingcapabilities.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <tsendingcapabilities.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22tsendingcapabilities.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-565-versioninfo-h-md"></a>
## versioninfo.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/565_versioninfo_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-565-versioninfo-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `versioninfo.h`

#### Vai trò

Data/value object or record definitions used by the API.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Version Info API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-113-version-info-api-md)

#### Cách include

```cpp
#include <versioninfo.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\versioninfo.h"
findstr /s /n /i /c:"#include <versioninfo.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"versioninfo.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: versioninfo.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <versioninfo.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22versioninfo.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)

---

<a id="src-20-public-header-reference-566-vibractrl-h-md"></a>
## vibractrl.h

- Original corpus path: `20_PUBLIC_HEADER_REFERENCE/566_vibractrl_h.md`
- Consolidated reference: `58-header-reference-526-566.md#src-20-public-header-reference-566-vibractrl-h-md`
- `kind`: public_header_reference
- `target`: S60 3rd Edition FP2

### `vibractrl.h`

#### Vai trò

Public declaration header. Inspect classes, exported functions, ownership comments and availability guards.

- Loại: **C/C++ public header**
- Được Quick Reference liệt kê trong: [Vibra Control API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-115-vibra-control-api-md)

#### Cách include

```cpp
#include <vibractrl.h>
```

Với `.hrh`, file thường được include từ `.rss` và/hoặc header C++ để command/resource ID
không lệch nhau.

#### Cách khám phá toàn bộ symbol trong SDK thật

```bat
findstr /n /i /r "class struct enum IMPORT_C EXPORT_C" "%EPOCROOT%epoc32\include\vibractrl.h"
findstr /s /n /i /c:"#include <vibractrl.h>" C:\S60\devices\*.cpp C:\S60\devices\*.h
findstr /s /n /i /c:"vibractrl.h" C:\S60\devices\*.mmp C:\S60\devices\*.rss
```

Sau đó ghi lại cho từng symbol:

```yaml
symbol: COrRClassName
header: vibractrl.h
library: resolve_from_class_page_or_mmp
capabilities: resolve_per_member
ownership: NewL_delete | Open_Close | borrowed | transferred
async: true_or_false
cancel: exact_cancel_method
availability: base_sdk | api_plugin | device_extension
```

#### Compile probe

```cpp
#include <e32base.h>
#include <vibractrl.h>

GLDEF_C TInt E32Main()
{
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup) return KErrNoMemory;
    // Reference one exact public symbol from this header here.
    delete cleanup;
    return KErrNone;
}
```

Header compile thành công chưa chứng minh library, capability hay implementation trên máy
thật. Tiếp tục bằng linker probe và device test.

#### Bẫy

- Header cùng tên có thể khác method/overload giữa FP2, S60 5th và Symbian^3.
- Một số header được API Plug-in chép vào include tree; đừng gắn nhãn base SDK nếu chưa
  so sánh installation manifest.
- Internal/private header không phải compatibility contract cho app cộng đồng.
- Macro/conditional compile có thể ẩn symbol theo platform build.

#### Link tham chiếu

- [Official public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Header search locator](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22vibractrl.h%22)
- [Developer’s Library guide](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
