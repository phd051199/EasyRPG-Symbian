# Complete API usage guides 001–030

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-10-s60-quick-reference-apis-001-aiw-criteria-api-md"></a>
## AIW Criteria API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/001_aiw_criteria_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-001-aiw-criteria-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### AIW Criteria API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng criteria/interest resource để mô tả command/content/provider AIW cần tìm.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Defines AIW criteria records used to identify commands, MIME types and providers.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `TAiwCriteriaItem`, `AIW command/content constants`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aiwcommon.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `aiwcommon.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aiwcommon.h>
#include <aiwcommon.hrh>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là R-class session/subsession, synchronous utility hoặc asynchronous server. Ownership và capability được xác định ở member level.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Khai báo interest/criteria trong resource hoặc C++.
2. Chọn command ID, service class/content type.
3. Attach vào handler.
4. Để framework resolve providers.
5. Fallback khi không provider.

#### 6. Build metadata

##### Import libraries

- Xác minh cùng AIW framework

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"aiwcommon.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"TAiwCriteriaItem" C:\S60\devices\*.mmp
```

##### Capabilities

Theo service.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
RServiceSession session;
User::LeaveIfError(session.Connect());
CleanupClosePushL(session);
// Open subsession / issue operation using the exact documented types.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Đóng parent session trước subsession.
- Descriptor request hết lifetime.
- Thiếu capability member cụ thể.
- Poll thay notification.
- Dùng API deprecated dù FP2 có replacement.
- Không hard-code provider UID nếu criteria đủ biểu đạt.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — AIW Criteria API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AIW_Criteria_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22AIW+Criteria+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md"></a>
## AIW Generic Parameter API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/002_aiw_generic_parameter_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### AIW Generic Parameter API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng generic param list/variant để trao đổi typed parameters giữa AIW consumer/provider.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Carries typed input and output parameters between AIW consumers and providers.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CAiwGenericParamList`, `TAiwGenericParam`, `TAiwVariant`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aiwgenericparam.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aiwgenericparam.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `aiwvariant.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aiwvarianttype.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aiwgenericparam.h>
#include <aiwgenericparam.hrh>
#include <aiwvariant.h>
#include <aiwvarianttype.hrh>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là R-class session/subsession, synchronous utility hoặc asynchronous server. Ownership và capability được xác định ở member level.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Tạo input/output lists.
2. Append parameter với semantic ID + typed variant.
3. Giữ descriptor/object lifetime theo copy semantics.
4. Execute service.
5. Đọc output params và kiểm tra type.

#### 6. Build metadata

##### Import libraries

- Xác minh cùng AIW service handler

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"aiwgenericparam.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CAiwGenericParamList" C:\S60\devices\*.mmp
```

##### Capabilities

Theo service.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
RServiceSession session;
User::LeaveIfError(session.Connect());
CleanupClosePushL(session);
// Open subsession / issue operation using the exact documented types.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Đóng parent session trước subsession.
- Descriptor request hết lifetime.
- Thiếu capability member cụ thể.
- Poll thay notification.
- Dùng API deprecated dù FP2 có replacement.
- Không cast variant mù; semantic ID và variant type phải khớp.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — AIW Generic Parameter API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AIW_Generic_Parameter_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22AIW+Generic+Parameter+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-003-aiw-service-handler-api-md"></a>
## AIW Service Handler API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/003_aiw_service_handler_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-003-aiw-service-handler-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### AIW Service Handler API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng AIW để khám phá/invoke service provider theo criteria thay vì hard-code app UID/provider.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Consumer-side entry point for locating and invoking AIW services.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CAiwServiceHandler`, `CAiwGenericParamList`, `TAiwGenericParam`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aiwservicehandler.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aiwservicehandler.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là R-class session/subsession, synchronous utility hoặc asynchronous server. Ownership và capability được xác định ở member level.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Construct service handler.
2. Attach menu/interest resource hoặc criteria.
3. Initialize menu pane.
4. Execute service command với in/out params.
5. Handle async callback và detach interest.

#### 6. Build metadata

##### Import libraries

- servicehandler.lib (xác minh SDK)

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"aiwservicehandler.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CAiwServiceHandler" C:\S60\devices\*.mmp
```

##### Capabilities

Theo provider command.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
RServiceSession session;
User::LeaveIfError(session.Connect());
CleanupClosePushL(session);
// Open subsession / issue operation using the exact documented types.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Đóng parent session trước subsession.
- Descriptor request hết lifetime.
- Thiếu capability member cụ thể.
- Poll thay notification.
- Dùng API deprecated dù FP2 có replacement.
- Không assume provider tồn tại; params/type IDs phải khớp contract.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — AIW Service Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AIW_Service_Handler_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22AIW+Service+Handler+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-004-access-point-engine-api-md"></a>
## Access Point Engine API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/004_access_point_engine_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-004-access-point-engine-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 3rd Ed FP2

### Access Point Engine API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy access-point database access and selection helpers. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy access-point database access and selection helpers.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 3rd Ed FP2**
- Nhận xét: API được đánh dấu deprecated từ **S60 3rd Ed FP2**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `apselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aplistitem.h` | Data/value object or record definitions used by the API. |
| `aputils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `apengineconsts.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `activeapdb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aplistitemlist.h` | Data/value object or record definitions used by the API. |
| `apenginever.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `apnetworkitem.h` | Data/value object or record definitions used by the API. |
| `apnetworks.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `apnetworkitemlist.h` | Data/value object or record definitions used by the API. |
| `apaccesspointitem.h` | Data/value object or record definitions used by the API. |
| `apdatahandler.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <apselect.h>
#include <aplistitem.h>
#include <aputils.h>
#include <apengineconsts.h>
#include <activeapdb.h>
#include <aplistitemlist.h>
#include <apenginever.h>
#include <apnetworkitem.h>
#include <apnetworks.h>
#include <apnetworkitemlist.h>
#include <apaccesspointitem.h>
#include <apdatahandler.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"apselect.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"apselect.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Access Point Engine API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Access_Point_Engine_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Access+Point+Engine+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-005-access-point-settings-handler-api-md"></a>
## Access Point Settings Handler API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/005_access_point_settings_handler_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-005-access-point-settings-handler-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 3rd Ed FP2

### Access Point Settings Handler API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy UI for choosing or editing an access point. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy UI for choosing or editing an access point.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 3rd Ed FP2**
- Nhận xét: API được đánh dấu deprecated từ **S60 3rd Ed FP2**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `apsettingshandlercommons.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `apsettingshandlerui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <apsettingshandlercommons.h>
#include <apsettingshandlerui.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"apsettingshandlercommons.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"apsettingshandlercommons.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Access Point Settings Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Access_Point_Settings_Handler_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Access+Point+Settings+Handler+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-006-animation-api-md"></a>
## Animation API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/006_animation_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-006-animation-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Animation API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Bitmap-animation support for Avkon controls. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Bitmap-animation support for Avkon controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknanim.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `aknbitmapanimation.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknanim.hrh>
#include <aknbitmapanimation.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"aknanim.hrh" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknanim.hrh" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Animation API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Animation_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Animation+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-007-application-framework-api-md"></a>
## Application Framework API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/007_application_framework_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-007-application-framework-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Application Framework API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng để dựng lifecycle chuẩn của app Avkon: Application → Document → AppUi/ViewAppUi → View/Control. Không tự viết event loop/window manager.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Core Avkon application, document, AppUi and view classes.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CAknApplication`, `CAknDocument`, `CAknAppUi`, `CAknViewAppUi`, `CAknView`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknapp.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknappui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `akndoc.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknview.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknviewappui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknapp.h>
#include <aknappui.h>
#include <akndoc.h>
#include <aknview.h>
#include <aknviewappui.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Khai báo UID2/UID3 và registration resource.
2. Tạo CAknApplication và CAknDocument theo two-phase construction.
3. Trong AppUi gọi BaseConstructL(), tạo control hoặc đăng ký views.
4. Đưa control nhận phím vào control stack.
5. Xử lý command, foreground/background và cleanup theo thứ tự ngược.

#### 6. Build metadata

##### Import libraries

- `euser.lib`
- `apparc.lib`
- `cone.lib`
- `eikcore.lib`
- `avkon.lib`

```text
LIBRARY         euser.lib
LIBRARY         apparc.lib
LIBRARY         cone.lib
LIBRARY         eikcore.lib
LIBRARY         avkon.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"aknapp.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CAknApplication" C:\S60\devices\*.mmp
```

##### Capabilities

None cho lifecycle/UI cơ bản; capability phát sinh từ dịch vụ app gọi.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
class CMyAppUi : public CAknAppUi
{
public:
    void ConstructL()
    {
        BaseConstructL(EAknEnableSkin);
        iContainer = CMyContainer::NewL(ClientRect());
        AddToStackL(iContainer);
    }
    ~CMyAppUi()
    {
        if (iContainer) { RemoveFromStack(iContainer); delete iContainer; }
    }
    void HandleCommandL(TInt aCommand)
    {
        if (aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit) Exit();
    }
private:
    CMyContainer* iContainer;
};
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.
- Không tự chạy vòng lặp sự kiện, không giữ control đã bị view deactivation xoá, không delete this khi thoát.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Application Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Application_Framework_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Application+Framework+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-008-audio-effects-api-md"></a>
## Audio Effects API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/008_audio_effects_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Audio Effects API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Equalizer, reverb, widening, spatial and other audio-effect control interfaces. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Equalizer, reverb, widening, spatial and other audio-effect control interfaces.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `audioeffectbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `audioeffectdata.h` | Data/value object or record definitions used by the API. |
| `maudioeffectobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `audioequalizerbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `audioequalizerdata.h` | Data/value object or record definitions used by the API. |
| `maudioequalizerobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `bassboostbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `bassboostdata.h` | Data/value object or record definitions used by the API. |
| `mbassboostobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `distanceattenuationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `distanceattenuationdata.h` | Data/value object or record definitions used by the API. |
| `mdistanceattenuationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `dopplerbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `dopplerdata.h` | Data/value object or record definitions used by the API. |
| `mdopplerobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `listenerdopplerbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mlistenerdopplerobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `sourcedopplerbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `msourcedopplerobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `environmentalreverbbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `environmentalreverbdata.h` | Data/value object or record definitions used by the API. |
| `menvironmentalreverbobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `locationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `locationdata.h` | Data/value object or record definitions used by the API. |
| `mlocationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `listenerlocationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mlistenerlocationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `sourcelocationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `msourcelocationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `loudnessbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `loudnessdata.h` | Data/value object or record definitions used by the API. |
| `mloudnessobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `orientationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `orientationdata.h` | Data/value object or record definitions used by the API. |
| `morientationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `listenerorientationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mlistenerorientationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `sourceorientationbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `msourceorientationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `stereowideningbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `stereowideningdata.h` | Data/value object or record definitions used by the API. |
| `mstereowideningobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `roomlevelbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `roomleveldata.h` | Data/value object or record definitions used by the API. |
| `mroomlevelobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <audioeffectbase.h>
#include <audioeffectdata.h>
#include <maudioeffectobserver.h>
#include <audioequalizerbase.h>
#include <audioequalizerdata.h>
#include <maudioequalizerobserver.h>
#include <bassboostbase.h>
#include <bassboostdata.h>
#include <mbassboostobserver.h>
#include <distanceattenuationbase.h>
#include <distanceattenuationdata.h>
#include <mdistanceattenuationobserver.h>
// + 33 header khác; chỉ include header chứa symbol thực sự dùng.
```

#### 4. Mô hình kiến trúc và ownership

Hầu hết multimedia/network-heavy APIs là asynchronous utility hoặc client-server. Observer callback báo open/prepare/play/convert completion. Buffer/file/bitmap phải sống tới callback.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Tạo utility với observer.
2. Open/load/reserve bất đồng bộ.
3. Chờ callback thành công.
4. Prepare/configure.
5. Start operation.
6. Cancel/stop và release resource khi background/exit.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"audioeffectbase.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"audioeffectbase.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: operation completion arrives through the documented observer.
iUtility = CMediaUtility::NewL(*this);
iUtility->OpenL(aSource);
// Continue only in the open-complete callback.
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Gọi Play/Convert trước open callback.
- Buffer tạm hết lifetime.
- Không xử lý KErrNotSupported/codec thiếu.
- Giữ camera/audio resource khi background.
- Tin emulator thay cho device codec test.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Audio Effects API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Audio_Effects_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Audio+Effects+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-009-audio-effects-presets-api-md"></a>
## Audio Effects Presets API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/009_audio_effects_presets_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-009-audio-effects-presets-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Audio Effects Presets API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Simplified preset-based control for selected audio effects. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Simplified preset-based control for selected audio effects.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `audioequalizerutility.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `audioequalizerutilitydata.h` | Data/value object or record definitions used by the API. |
| `environmentalreverbutility.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `environmentalreverbutilitydata.h` | Data/value object or record definitions used by the API. |
| `stereowideningutility.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `stereowideningutilitydata.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <audioequalizerutility.h>
#include <audioequalizerutilitydata.h>
#include <environmentalreverbutility.h>
#include <environmentalreverbutilitydata.h>
#include <stereowideningutility.h>
#include <stereowideningutilitydata.h>
```

#### 4. Mô hình kiến trúc và ownership

Hầu hết multimedia/network-heavy APIs là asynchronous utility hoặc client-server. Observer callback báo open/prepare/play/convert completion. Buffer/file/bitmap phải sống tới callback.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Tạo utility với observer.
2. Open/load/reserve bất đồng bộ.
3. Chờ callback thành công.
4. Prepare/configure.
5. Start operation.
6. Cancel/stop và release resource khi background/exit.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"audioequalizerutility.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"audioequalizerutility.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: operation completion arrives through the documented observer.
iUtility = CMediaUtility::NewL(*this);
iUtility->OpenL(aSource);
// Continue only in the open-complete callback.
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Gọi Play/Convert trước open callback.
- Buffer tạm hết lifetime.
- Không xử lý KErrNotSupported/codec thiếu.
- Giữ camera/audio resource khi background.
- Tin emulator thay cho device codec test.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Audio Effects Presets API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Audio_Effects_Presets_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Audio+Effects+Presets+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-010-blid-application-satellite-info-api-md"></a>
## BLID Application Satellite Info API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/010_blid_application_satellite_info_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-010-blid-application-satellite-info-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### BLID Application Satellite Info API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Displays satellite information through a platform UI. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Displays satellite information through a platform UI.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `csatelliteinfoui.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <csatelliteinfoui.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"csatelliteinfoui.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"csatelliteinfoui.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — BLID Application Satellite Info API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/BLID_Application_Satellite_Info_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22BLID+Application+Satellite+Info+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-011-base-controls-api-md"></a>
## Base Controls API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/011_base_controls_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-011-base-controls-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Base Controls API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Base classes and alignment types used by S60 UI controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Base classes and alignment types used by S60 UI controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikalign.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikbctrl.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akncontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikalign.h>
#include <eikbctrl.h>
#include <akncontrol.h>
```

#### 4. Mô hình kiến trúc và ownership

UI chạy trong thread có Active Scheduler và CONE environment. Nhiều object được tạo bằng NewL/NewLC hoặc resource constructor; ownership thường thuộc AppUi/container/dialog sau một bước attach cụ thể.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Xác định control/dialog/resource type.
2. Khai báo `.rss/.hrh` nếu API yêu cầu.
3. ConstructL/CreateWindowL rồi đặt rect.
4. Đưa control nhận phím vào control stack.
5. Xử lý command/key/event và redraw tối thiểu.
6. Remove khỏi stack trước khi delete.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"eikalign.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikalign.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the API-specific control/resource declared in the headers above.
CMyControl* control = CMyControl::NewL(/* documented arguments */);
CleanupStack::PushL(control);
// Configure using the exact FP2 member signatures.
CleanupStack::Pop(control); // transfer ownership only when the API documents it
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Dùng touch-only API trên E72 keypad.
- Vẽ hard-code màu thay vì skin/layout.
- Sai resource type hoặc command ID.
- Delete dialog sau RunLD/ExecuteLD dù API đã tự huỷ.
- Giữ pointer tới model/control đã bị framework thay thế.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Base Controls API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Base_controls_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Base+Controls+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-012-browser-control-api-md"></a>
## Browser Control API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/012_browser_control_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-012-browser-control-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP3
- `deprecated`: none-listed

### Browser Control API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng browser control khi cần render web nhúng; không tự viết HTML engine.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Embeddable browser control and its observer/provider interfaces.

- First published: **S60 2nd Ed FP3**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CBrCtlInterface`, `MBrCtlSoftkeysObserver`, `MBrCtlLinkResolver`, `MBrCtlSpecialLoadObserver`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `brctlinterface.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `brctldialogsprovider.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `brctllayoutobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `brctllinkresolver.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `brctlsoftkeysobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `brctlspecialloadobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `brctldefs.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `brctldownloadobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `brctlwindowobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `browseruisdkcrkeys.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <brctlinterface.h>
#include <brctldialogsprovider.h>
#include <brctllayoutobserver.h>
#include <brctllinkresolver.h>
#include <brctlsoftkeysobserver.h>
#include <brctlspecialloadobserver.h>
#include <brctldefs.h>
#include <brctldownloadobserver.h>
#include <brctlwindowobserver.h>
#include <browseruisdkcrkeys.h>
```

#### 4. Mô hình kiến trúc và ownership

Hầu hết multimedia/network-heavy APIs là asynchronous utility hoặc client-server. Observer callback báo open/prepare/play/convert completion. Buffer/file/bitmap phải sống tới callback.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. CreateBrowserControlL với capabilities/command IDs.
2. Cài observer/provider.
3. SetRect/ActivateL.
4. LoadUrlL/LoadDataL.
5. Handle commands, downloads, special schemes và cleanup.

#### 6. Build metadata

##### Import libraries

- Xác minh local SDK/browser control package

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"brctlinterface.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CBrCtlInterface" C:\S60\devices\*.mmp
```

##### Capabilities

NetworkServices và file/data tùy load.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: operation completion arrives through the documented observer.
iUtility = CMediaUtility::NewL(*this);
iUtility->OpenL(aSource);
// Continue only in the open-complete callback.
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Gọi Play/Convert trước open callback.
- Buffer tạm hết lifetime.
- Không xử lý KErrNotSupported/codec thiếu.
- Giữ camera/audio resource khi background.
- Tin emulator thay cho device codec test.
- Không giả định engine hỗ trợ web hiện đại/TLS mới; không block callback.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Browser Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Browser_Control_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Browser+Control+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-013-browser-plug-in-api-md"></a>
## Browser Plug-in API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/013_browser_plug_in_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP2
- `deprecated`: none-listed

### Browser Plug-in API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy NPAPI-style browser plug-in interfaces. Dùng khi cần mở rộng platform qua ECom/provider/observer contract thay vì hard-code implementation vào app.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy NPAPI-style browser plug-in interfaces.

- First published: **S60 2nd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `npupp.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `pluginadapterinterface.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `jri.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `npapi.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `prcpucfg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `prtypes.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cecombrowserplugininterface.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `browserplugininterface.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <npupp.h>
#include <pluginadapterinterface.h>
#include <jri.h>
#include <npapi.h>
#include <prcpucfg.h>
#include <prtypes.h>
#include <cecombrowserplugininterface.h>
#include <browserplugininterface.h>
```

#### 4. Mô hình kiến trúc và ownership

Consumer dùng interface/factory; implementation được chọn bằng UID/criteria/resource. Lifecycle thường gồm discovery → create → invoke/callback → release → REComSession::FinalClose ở điểm an toàn.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Xác định consumer hay provider.
2. Đọc interface UID/implementation UID/resource requirements.
3. Tạo implementation qua factory/ECom.
4. Implement observer/callback không block.
5. Đóng interface trước FinalClose.
6. Test khi implementation không tồn tại.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"npupp.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"npupp.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: exact factory and destructor-key handling vary by API.
MService* service = ServiceFactory::NewL(/* criteria/UID */);
CleanupStack::PushL(service);
service->StartL(/* observer */);
CleanupStack::PopAndDestroy(service);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Sai UID/resource registration.
- Export factory signature sai.
- Gọi FinalClose khi object ECom còn sống.
- Giả định plugin có trên mọi firmware.
- Để callback dùng object owner đã bị destroy.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Browser Plug-in API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Browser_Plug-in_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Browser+Plug-in+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-014-buttons-api-md"></a>
## Buttons API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/014_buttons_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Buttons API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Button groups, command buttons and CBA controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Button groups, command buttons and CBA controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikbgfty.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikbtgpc.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikbutb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikcba.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikcmbut.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbbut.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikmnbut.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikbtgps.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikbtgrp.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikbgfty.h>
#include <eikbtgpc.h>
#include <eikbutb.h>
#include <eikcba.h>
#include <eikcmbut.h>
#include <eiklbbut.h>
#include <eikmnbut.h>
#include <eikbtgps.h>
#include <eikbtgrp.h>
```

#### 4. Mô hình kiến trúc và ownership

UI chạy trong thread có Active Scheduler và CONE environment. Nhiều object được tạo bằng NewL/NewLC hoặc resource constructor; ownership thường thuộc AppUi/container/dialog sau một bước attach cụ thể.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Xác định control/dialog/resource type.
2. Khai báo `.rss/.hrh` nếu API yêu cầu.
3. ConstructL/CreateWindowL rồi đặt rect.
4. Đưa control nhận phím vào control stack.
5. Xử lý command/key/event và redraw tối thiểu.
6. Remove khỏi stack trước khi delete.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"eikbgfty.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikbgfty.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the API-specific control/resource declared in the headers above.
CMyControl* control = CMyControl::NewL(/* documented arguments */);
CleanupStack::PushL(control);
// Configure using the exact FP2 member signatures.
CleanupStack::Pop(control); // transfer ownership only when the API documents it
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Dùng touch-only API trên E72 keypad.
- Vẽ hard-code màu thay vì skin/layout.
- Sai resource type hoặc command ID.
- Delete dialog sau RunLD/ExecuteLD dù API đã tự huỷ.
- Giữ pointer tới model/control đã bị framework thay thế.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Buttons API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Buttons_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Buttons+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-015-calendar-api-md"></a>
## Calendar API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/015_calendar_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-015-calendar-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Calendar API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy calendar observer and UI-related definitions. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy calendar observer and UI-related definitions.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikcal.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikcal.h>
```

#### 4. Mô hình kiến trúc và ownership

Thường là client-server/session + object model. Data có ID và transaction/lock semantics; một số operation trả active operation để chạy bất đồng bộ.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Mở session/database.
2. Tạo view/query/iterator thay vì load toàn bộ.
3. Read/open record.
4. Sửa qua edit object/transaction.
5. Commit hoặc rollback.
6. Close session sau child handles.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"eikcal.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikcal.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: prefer the API query/view/iterator instead of scanning all records.
RDataSession session;
User::LeaveIfError(session.Open());
CleanupClosePushL(session);
// Query/read/update using exact FP2 classes.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Giữ record/view qua database change.
- Không xử lý lock/conflict.
- Bỏ transaction cho batch update.
- Đọc toàn DB trên UI thread.
- Thiếu ReadUserData/WriteUserData.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Calendar API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Calendar_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Calendar+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-016-calendar-interim-utils2-api-md"></a>
## Calendar Interim Utils2 API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/016_calendar_interim_utils2_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-016-calendar-interim-utils2-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Calendar Interim Utils2 API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Additional helpers for the Calendar Interim API. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Additional helpers for the Calendar Interim API.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `caleninterimutils2.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <caleninterimutils2.h>
```

#### 4. Mô hình kiến trúc và ownership

Thường là client-server/session + object model. Data có ID và transaction/lock semantics; một số operation trả active operation để chạy bất đồng bộ.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Mở session/database.
2. Tạo view/query/iterator thay vì load toàn bộ.
3. Read/open record.
4. Sửa qua edit object/transaction.
5. Commit hoặc rollback.
6. Close session sau child handles.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"caleninterimutils2.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"caleninterimutils2.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: prefer the API query/view/iterator instead of scanning all records.
RDataSession session;
User::LeaveIfError(session.Open());
CleanupClosePushL(session);
// Query/read/update using exact FP2 classes.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Giữ record/view qua database change.
- Không xử lý lock/conflict.
- Bỏ transaction cho batch update.
- Đọc toàn DB trên UI thread.
- Thiếu ReadUserData/WriteUserData.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Calendar Interim Utils2 API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Calendar_Interim_Utils2_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Calendar+Interim+Utils2+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-017-central-repository-notification-handler-api-md"></a>
## Central Repository Notification Handler API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/017_central_repository_notification_handler_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-017-central-repository-notification-handler-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP1
- `deprecated`: none-listed

### Central Repository Notification Handler API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng notification handler/observer thay vì polling setting keys.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Active-object wrapper for Central Repository notifications.

- First published: **S60 3rd Ed FP1**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CCenRepNotifyHandler`, `MCenRepNotifyHandlerCallback`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cenrepnotifyhandler.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cenrepnotifyhandler.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Open CRepository.
2. Create handler với key/type/callback.
3. StartListeningL.
4. Handle value/error callback nhanh.
5. StopListening trước destroy.

#### 6. Build metadata

##### Import libraries

- centralrepository.lib + notification helper library, xác minh SDK

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"cenrepnotifyhandler.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CCenRepNotifyHandler" C:\S60\devices\*.mmp
```

##### Capabilities

Theo repository key policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.
- Không đọc/write lại key vô hạn trong callback; tránh feedback loop.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Central Repository Notification Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Central_Repository_Notification_Handler_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Central+Repository+Notification+Handler+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-018-clock-model-observer-api-md"></a>
## Clock Model Observer API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/018_clock_model_observer_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-018-clock-model-observer-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 3rd Ed FP1

### Clock Model Observer API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Observer interface for legacy clock model changes. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Observer interface for legacy clock model changes.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 3rd Ed FP1**
- Nhận xét: API được đánh dấu deprecated từ **S60 3rd Ed FP1**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `clkmdlobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <clkmdlobserver.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"clkmdlobserver.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"clkmdlobserver.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Clock Model Observer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Clock_Model_Observer_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Clock+Model+Observer+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md"></a>
## Common File Dialogs API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/019_common_file_dialogs_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Common File Dialogs API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng standard file/memory/filename dialogs để có UI và filter nhất quán.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Standard file, filename and drive-selection dialogs.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CAknFileSelectionDialog`, `CAknMemorySelectionDialog`, `CAknFileNamePromptDialog`, `MAknFileFilter`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akncommondialogs.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `cakncommondialogsbase.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `caknfilenamepromptdialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `caknfileselectiondialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `caknmemoryselectiondialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `caknmemoryselectionsettingitem.h` | Data/value object or record definitions used by the API. |
| `caknmemoryselectionsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `commondialogs.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `maknfilefilter.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `maknfileselectionobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `maknmemoryselectionobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `akncommondialogsdynmem.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `caknmemoryselectiondialogmultidrive.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `caknmemoryselectionsettingitemmultidrive.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akncommondialogs.h>
#include <cakncommondialogsbase.h>
#include <caknfilenamepromptdialog.h>
#include <caknfileselectiondialog.h>
#include <caknmemoryselectiondialog.h>
#include <caknmemoryselectionsettingitem.h>
#include <caknmemoryselectionsettingpage.h>
#include <commondialogs.hrh>
#include <maknfilefilter.h>
#include <maknfileselectionobserver.h>
#include <maknmemoryselectionobserver.h>
#include <akncommondialogsdynmem.h>
// + 2 header khác; chỉ include header chứa symbol thực sự dùng.
```

#### 4. Mô hình kiến trúc và ownership

UI chạy trong thread có Active Scheduler và CONE environment. Nhiều object được tạo bằng NewL/NewLC hoặc resource constructor; ownership thường thuộc AppUi/container/dialog sau một bước attach cụ thể.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn dialog đúng use-case.
2. Tạo filter nếu cần.
3. RunLD/Execute và kiểm tra cancel.
4. Chuẩn hóa path bằng PathInfo/TParse.
5. Kiểm tra quyền trước mở file.

#### 6. Build metadata

##### Import libraries

- commonui.lib (xác minh local SDK)

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"akncommondialogs.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CAknFileSelectionDialog" C:\S60\devices\*.mmp
```

##### Capabilities

Phụ thuộc đường dẫn file.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the API-specific control/resource declared in the headers above.
CMyControl* control = CMyControl::NewL(/* documented arguments */);
CleanupStack::PushL(control);
// Configure using the exact FP2 member signatures.
CleanupStack::Pop(control); // transfer ownership only when the API documents it
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Dùng touch-only API trên E72 keypad.
- Vẽ hard-code màu thay vì skin/layout.
- Sai resource type hoặc command ID.
- Delete dialog sau RunLD/ExecuteLD dù API đã tự huỷ.
- Giữ pointer tới model/control đã bị framework thay thế.
- Không tự viết file browser nếu standard dialog đủ dùng.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Common File Dialogs API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Common_File_Dialogs_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Common+File+Dialogs+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-020-cone-resource-loader-api-md"></a>
## Cone Resource Loader API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/020_cone_resource_loader_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-020-cone-resource-loader-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Cone Resource Loader API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Loads and unloads localized resource files in the CONE environment. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Loads and unloads localized resource files in the CONE environment.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `coneresloader.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <coneresloader.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"coneresloader.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"coneresloader.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Cone Resource Loader API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cone_Resource_Loader_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Cone+Resource+Loader+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-021-connection-monitor-server-api-md"></a>
## Connection Monitor Server API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/021_connection_monitor_server_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-021-connection-monitor-server-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP2
- `deprecated`: none-listed

### Connection Monitor Server API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng để quan sát connection hiện hữu và attributes/events; không poll socket/network trạng thái bằng timer.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Observes active data connections and selected connection attributes.

- First published: **S60 2nd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `RConnectionMonitor`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `rconnmon.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <rconnmon.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là R-class session/subsession, synchronous utility hoặc asynchronous server. Ownership và capability được xác định ở member level.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. ConnectL/Connect.
2. Register observer.
3. Enumerate/query connection IDs.
4. Subscribe events.
5. Cancel/Close khi teardown.

#### 6. Build metadata

##### Import libraries

- `connmon.lib`

```text
LIBRARY         connmon.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"rconnmon.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"RConnectionMonitor" C:\S60\devices\*.mmp
```

##### Capabilities

NetworkServices và/hoặc operation-specific.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
RServiceSession session;
User::LeaveIfError(session.Connect());
CleanupClosePushL(session);
// Open subsession / issue operation using the exact documented types.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Đóng parent session trước subsession.
- Descriptor request hết lifetime.
- Thiếu capability member cụ thể.
- Poll thay notification.
- Dùng API deprecated dù FP2 có replacement.
- Không coi event là đảm bảo Internet reachability; chỉ phản ánh connection layer.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Connection Monitor Server API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Monitor_Server_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Connection+Monitor+Server+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-022-connection-settings-api-md"></a>
## Connection Settings API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/022_connection_settings_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-022-connection-settings-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### Connection Settings API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng để đọc destinations và connection methods theo mô hình mới của FP2. Đây là API đồng bộ và tài liệu usage mô tả giao diện đọc; không nên quay lại Access Point Engine đã deprecated.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Reads and manages connection methods and destinations without UI.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `RCmManager`, `RCmDestination`, `RCmConnectionMethod`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cmmanager.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `cmmanagerdef.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `cmdestination.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cmconnectionmethod.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cmconnectionmethoddef.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cmpluginpacketdatadef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `cmplugindialcommondefs.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `cmplugincsddef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `cmpluginhscsddef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `cmpluginembdestinationdef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `cmpluginvpndef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `cmpluginwlandef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `cmdefconnvalues.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cmmanager.h>
#include <cmmanagerdef.h>
#include <cmdestination.h>
#include <cmconnectionmethod.h>
#include <cmconnectionmethoddef.h>
#include <cmpluginpacketdatadef.h>
#include <cmplugindialcommondefs.h>
#include <cmplugincsddef.h>
#include <cmpluginhscsddef.h>
#include <cmpluginembdestinationdef.h>
#include <cmpluginvpndef.h>
#include <cmpluginwlandef.h>
// + 1 header khác; chỉ include header chứa symbol thực sự dùng.
```

#### 4. Mô hình kiến trúc và ownership

Có thể là R-class session/subsession, synchronous utility hoặc asynchronous server. Ownership và capability được xác định ở member level.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. RCmManager::OpenLC().
2. Liệt kê destination IDs bằng AllDestinationsL().
3. Mở RCmDestination theo ID.
4. Đọc connection methods/attributes.
5. Close/cleanup R-class theo thứ tự ngược.

#### 6. Build metadata

##### Import libraries

- `cmmanager.lib`

```text
LIBRARY         cmmanager.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"cmmanager.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"RCmManager" C:\S60\devices\*.mmp
```

##### Capabilities

Đọc thuộc tính private user data có thể cần ReadDeviceData; thuộc tính thông thường phụ thuộc member cụ thể.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
RCmManager manager;
manager.OpenLC();
RArray<TUint32> ids;
CleanupClosePushL(ids);
manager.AllDestinationsL(ids);
for (TInt i = 0; i < ids.Count(); ++i)
{
    RCmDestination dest = manager.DestinationL(ids[i]);
    CleanupClosePushL(dest);
    // Read destination name and connection methods using FP2 declarations.
    CleanupStack::PopAndDestroy(&dest);
}
CleanupStack::PopAndDestroy(2, &manager); // ids, manager
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Đóng parent session trước subsession.
- Descriptor request hết lifetime.
- Thiếu capability member cụ thể.
- Poll thay notification.
- Dùng API deprecated dù FP2 có replacement.
- Không giả định API này tạo/xoá/sửa được mọi setting; official usage nêu giới hạn read-only cho luồng được mô tả. Không hard-code IAP ID giữa thiết bị.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Connection Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Connection+Settings+API%22)

##### API-specific subpages

- [S60_3_2_Connection_Settings_API_Specification_C2.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/S60_3_2_Connection_Settings_API_Specification_C2.html)
- [S60_3_2_Connection_Settings_API_Specification_C3.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/S60_3_2_Connection_Settings_API_Specification_C3.html)
- [S60_3_2_Connection_Settings_API_Specification_C4.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/S60_3_2_Connection_Settings_API_Specification_C4.html)
- [Connection_Settings_APIClassesIndexPage.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_APIClassesIndexPage.html)

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-023-connection-settings-ui-api-md"></a>
## Connection Settings UI API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/023_connection_settings_ui_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-023-connection-settings-ui-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### Connection Settings UI API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Platform UI for choosing a destination or connection method. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Platform UI for choosing a destination or connection method.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cmapplicationsettingsui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cmapplicationsettingsui.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là R-class session/subsession, synchronous utility hoặc asynchronous server. Ownership và capability được xác định ở member level.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Connect/Open/NewL.
2. Cấu hình request/criteria/options.
3. Issue synchronous call hoặc async request.
4. Xử lý callback/status/error.
5. Cancel outstanding request.
6. Close child trước parent.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"cmapplicationsettingsui.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cmapplicationsettingsui.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
RServiceSession session;
User::LeaveIfError(session.Connect());
CleanupClosePushL(session);
// Open subsession / issue operation using the exact documented types.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Đóng parent session trước subsession.
- Descriptor request hết lifetime.
- Thiếu capability member cụ thể.
- Poll thay notification.
- Dùng API deprecated dù FP2 có replacement.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Connection Settings UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_UI_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Connection+Settings+UI+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-024-content-listing-framework-api-md"></a>
## Content Listing Framework API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/024_content_listing_framework_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP1
- `deprecated`: none-listed

### Content Listing Framework API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng CLF để liệt kê/filter/sort/group media thay vì tự crawl toàn bộ filesystem.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Obtains, filters, groups and sorts media-item lists.

- First published: **S60 3rd Ed FP1**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `MCLFContentListingEngine`, `MCLFItemListModel`, `MCLFItem`, `MCLFSortingStyle`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `contentlistingfactory.h` | Factory entry point; normally creates an interface or implementation selected at runtime. |
| `clfcontentlisting.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `clfcontentlisting.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `mclfcontentlistingengine.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfitemlistmodel.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfcustomgrouper.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfcustomsorter.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfitem.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfchangeditemobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mclfmodifiableitem.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfoperationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mclfpostfilter.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfsortingstyle.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mclfprocessobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <contentlistingfactory.h>
#include <clfcontentlisting.h>
#include <clfcontentlisting.hrh>
#include <mclfcontentlistingengine.h>
#include <mclfitemlistmodel.h>
#include <mclfcustomgrouper.h>
#include <mclfcustomsorter.h>
#include <mclfitem.h>
#include <mclfchangeditemobserver.h>
#include <mclfmodifiableitem.h>
#include <mclfoperationobserver.h>
#include <mclfpostfilter.h>
// + 2 header khác; chỉ include header chứa symbol thực sự dùng.
```

#### 4. Mô hình kiến trúc và ownership

Thường là client-server/session + object model. Data có ID và transaction/lock semantics; một số operation trả active operation để chạy bất đồng bộ.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Create engine/model qua factory.
2. Set media types/filters/sorting.
3. Refresh asynchronously.
4. Observe operation completion/change.
5. Render model items; không giữ stale pointers.

#### 6. Build metadata

##### Import libraries

- Xác minh local SDK

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"contentlistingfactory.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"MCLFContentListingEngine" C:\S60\devices\*.mmp
```

##### Capabilities

ReadUserData/file policy tùy nguồn.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: prefer the API query/view/iterator instead of scanning all records.
RDataSession session;
User::LeaveIfError(session.Open());
CleanupClosePushL(session);
// Query/read/update using exact FP2 classes.
CleanupStack::PopAndDestroy(&session);
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Giữ record/view qua database change.
- Không xử lý lock/conflict.
- Bỏ transaction cho batch update.
- Đọc toàn DB trên UI thread.
- Thiếu ReadUserData/WriteUserData.
- Không scan ổ đĩa trên UI thread; không giả định metadata có đủ.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Content Listing Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Content_Listing_Framework_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Content+Listing+Framework+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-025-context-pane-api-md"></a>
## Context Pane API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/025_context_pane_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-025-context-pane-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Context Pane API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Controls the context pane in the status pane. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Controls the context pane in the status pane.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akncontext.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akncontext.h>
```

#### 4. Mô hình kiến trúc và ownership

UI chạy trong thread có Active Scheduler và CONE environment. Nhiều object được tạo bằng NewL/NewLC hoặc resource constructor; ownership thường thuộc AppUi/container/dialog sau một bước attach cụ thể.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Xác định control/dialog/resource type.
2. Khai báo `.rss/.hrh` nếu API yêu cầu.
3. ConstructL/CreateWindowL rồi đặt rect.
4. Đưa control nhận phím vào control stack.
5. Xử lý command/key/event và redraw tối thiểu.
6. Remove khỏi stack trước khi delete.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"akncontext.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akncontext.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the API-specific control/resource declared in the headers above.
CMyControl* control = CMyControl::NewL(/* documented arguments */);
CleanupStack::PushL(control);
// Configure using the exact FP2 member signatures.
CleanupStack::Pop(control); // transfer ownership only when the API documents it
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Dùng touch-only API trên E72 keypad.
- Vẽ hard-code màu thay vì skin/layout.
- Sai resource type hoặc command ID.
- Delete dialog sau RunLD/ExecuteLD dù API đã tự huỷ.
- Giữ pointer tới model/control đã bị framework thay thế.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Context Pane API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Context_Pane_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Context+Pane+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-026-drm-audio-player-api-md"></a>
## DRM Audio Player API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/026_drm_audio_player_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-026-drm-audio-player-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: S60 5th Ed

### DRM Audio Player API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: High-level playback helper for DRM-protected audio. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

High-level playback helper for DRM-protected audio.

- First published: **S60 3rd Ed**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `drmaudiosampleplayer.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <drmaudiosampleplayer.h>
```

#### 4. Mô hình kiến trúc và ownership

Hầu hết multimedia/network-heavy APIs là asynchronous utility hoặc client-server. Observer callback báo open/prepare/play/convert completion. Buffer/file/bitmap phải sống tới callback.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Tạo utility với observer.
2. Open/load/reserve bất đồng bộ.
3. Chờ callback thành công.
4. Prepare/configure.
5. Start operation.
6. Cancel/stop và release resource khi background/exit.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"drmaudiosampleplayer.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"drmaudiosampleplayer.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: operation completion arrives through the documented observer.
iUtility = CMediaUtility::NewL(*this);
iUtility->OpenL(aSource);
// Continue only in the open-complete callback.
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Gọi Play/Convert trước open callback.
- Buffer tạm hết lifetime.
- Không xử lý KErrNotSupported/codec thiếu.
- Giữ camera/audio resource khi background.
- Tin emulator thay cho device codec test.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — DRM Audio Player API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/DRM_Audio_Player_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22DRM+Audio+Player+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-027-drm-helper-api-md"></a>
## DRM Helper API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/027_drm_helper_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-027-drm-helper-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP2
- `deprecated`: none-listed

### DRM Helper API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Handles selected DRM errors and automated-content registration. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Handles selected DRM errors and automated-content registration.

- First published: **S60 2nd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `drmhelper.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `rdrmhelper.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `drmhelperserverinternalcrkeys.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <drmhelper.h>
#include <rdrmhelper.h>
#include <drmhelperserverinternalcrkeys.h>
```

#### 4. Mô hình kiến trúc và ownership

Hầu hết multimedia/network-heavy APIs là asynchronous utility hoặc client-server. Observer callback báo open/prepare/play/convert completion. Buffer/file/bitmap phải sống tới callback.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Tạo utility với observer.
2. Open/load/reserve bất đồng bộ.
3. Chờ callback thành công.
4. Prepare/configure.
5. Start operation.
6. Cancel/stop và release resource khi background/exit.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"drmhelper.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"drmhelper.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: operation completion arrives through the documented observer.
iUtility = CMediaUtility::NewL(*this);
iUtility->OpenL(aSource);
// Continue only in the open-complete callback.
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Gọi Play/Convert trước open callback.
- Buffer tạm hết lifetime.
- Không xử lý KErrNotSupported/codec thiếu.
- Giữ camera/audio resource khi background.
- Tin emulator thay cho device codec test.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — DRM Helper API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/DRM_Helper_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22DRM+Helper+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-028-drm-license-checker-api-md"></a>
## DRM License Checker API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/028_drm_license_checker_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-028-drm-license-checker-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP2
- `deprecated`: none-listed

### DRM License Checker API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Checks application or skin licensing conditions. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Checks application or skin licensing conditions.

- First published: **S60 2nd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `drmlicensechecker.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <drmlicensechecker.h>
```

#### 4. Mô hình kiến trúc và ownership

Hầu hết multimedia/network-heavy APIs là asynchronous utility hoặc client-server. Observer callback báo open/prepare/play/convert completion. Buffer/file/bitmap phải sống tới callback.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Tạo utility với observer.
2. Open/load/reserve bất đồng bộ.
3. Chờ callback thành công.
4. Prepare/configure.
5. Start operation.
6. Cancel/stop và release resource khi background/exit.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"drmlicensechecker.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"drmlicensechecker.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: operation completion arrives through the documented observer.
iUtility = CMediaUtility::NewL(*this);
iUtility->OpenL(aSource);
// Continue only in the open-complete callback.
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Gọi Play/Convert trước open callback.
- Buffer tạm hết lifetime.
- Không xử lý KErrNotSupported/codec thiếu.
- Giữ camera/audio resource khi background.
- Tin emulator thay cho device codec test.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — DRM License Checker API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/DRM_License_Checker_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22DRM+License+Checker+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-029-dialog-shutter-api-md"></a>
## Dialog Shutter API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/029_dialog_shutter_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-029-dialog-shutter-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Dialog Shutter API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Closes dialogs owned by an application. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Closes dialogs owned by an application.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akndlgshut.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akndlgshut.h>
```

#### 4. Mô hình kiến trúc và ownership

Có thể là static helper, value types, resource constants hoặc small client object. Đọc exact ownership vì utility APIs không đồng nhất.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Chọn đúng header/type.
2. Kiểm tra feature/version/return error.
3. Dùng platform result làm input cho business logic.
4. Giữ fallback.
5. Không cache kết quả runtime-sensitive quá lâu.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"akndlgshut.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akndlgshut.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the platform helper and handle unsupported results explicitly.
TRAPD(err, UsePlatformUtilityL());
if (err == KErrNotSupported || err == KErrNotFound)
    UseFallbackL();
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Hard-code dữ liệu platform.
- Bỏ qua KErrNotSupported.
- Dùng deprecated enum.
- Nhầm 8-bit/16-bit descriptor.
- Tin API tồn tại chỉ vì OS reference có trang.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Dialog Shutter API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Dialog_Shutter_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Dialog+Shutter+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.

---

<a id="src-10-s60-quick-reference-apis-030-dialogs-api-md"></a>
## Dialogs API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/030_dialogs_api.md`
- Consolidated reference: `21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-030-dialogs-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Dialogs API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Base dialog framework. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Base dialog framework.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikdialg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikdialg.h>
```

#### 4. Mô hình kiến trúc và ownership

UI chạy trong thread có Active Scheduler và CONE environment. Nhiều object được tạo bằng NewL/NewLC hoặc resource constructor; ownership thường thuộc AppUi/container/dialog sau một bước attach cụ thể.

Checklist ownership:

- `C*`: thường tạo bằng `NewL/NewLC`, owner phải `delete` trừ khi API ghi transfer ownership.
- `R*`: phải `Open/Connect/Create` rồi `Close`; child handle đóng trước parent session.
- `M*`: interface/observer, thường **không** được provider sở hữu; phải sống qua callback.
- hàm hậu tố `L`: có thể leave; dùng Cleanup Stack/TRAP ở boundary phù hợp.
- async request: buffer/descriptors/request object phải sống đến completion và phải có cancel path.

#### 5. Quy trình sử dụng chuẩn

1. Xác định control/dialog/resource type.
2. Khai báo `.rss/.hrh` nếu API yêu cầu.
3. ConstructL/CreateWindowL rồi đặt rect.
4. Đưa control nhận phím vào control stack.
5. Xử lý command/key/event và redraw tối thiểu.
6. Remove khỏi stack trước khi delete.

#### 6. Build metadata

##### Import libraries

- Chưa gán cứng: lấy từ class page, SDK example hoặc linker probe.

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"eikdialg.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikdialg.h" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific; đọc capability ở exact member page và server policy.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
// Pseudocode: use the API-specific control/resource declared in the headers above.
CMyControl* control = CMyControl::NewL(/* documented arguments */);
CleanupStack::PushL(control);
// Configure using the exact FP2 member signatures.
CleanupStack::Pop(control); // transfer ownership only when the API documents it
```

Ví dụ trên là pattern khởi đầu. Trước khi paste vào project, mở exact class/member page
và khớp signature với header trong SDK FP2 đã cài.

#### 8. Những chỗ dễ thiết kế lại bánh xe

- Tự viết discovery/selector thay vì dùng factory, AIW, ECom hoặc standard dialog.
- Tự poll thay vì dùng observer/notification API.
- Tự crawl database/filesystem thay vì query/model/content listing có sẵn.
- Tự parse metadata/protocol/file format trong khi platform đã có parser/engine.
- Tự dựng UI không theo Avkon resource/layout/skin.
- Tự lưu platform setting vào file riêng, làm mất tương thích với hệ thống.

#### 9. Lỗi và bẫy thường gặp

- Dùng touch-only API trên E72 keypad.
- Vẽ hard-code màu thay vì skin/layout.
- Sai resource type hoặc command ID.
- Delete dialog sau RunLD/ExecuteLD dù API đã tự huỷ.
- Giữ pointer tới model/control đã bị framework thay thế.

#### 10. Test matrix bắt buộc

| Kiểm tra | Mục tiêu |
|---|---|
| Header compile probe | Xác nhận declaration thật sự có trong SDK/plug-in đã cài. |
| WINSCW UDEB | Bắt leave/panic/resource lỗi nhanh. |
| GCCE UREL | Xác nhận import library và ARM build. |
| Install/sign | Xác nhận UID/SID/capability/certificate. |
| Nokia E72/FP2 device | Xác nhận firmware implementation, hardware và codec/service có thật. |
| Negative path | Cancel, timeout, no provider, unsupported, disk full, permission denied. |

#### 11. Link tài liệu đã tham chiếu

- [API index — Dialogs API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Dialogs_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Dialogs+API%22)

##### API-specific subpages

- Không có subpage URL được hard-code; mở API index rồi chọn **Purpose / API Description / Usage / Classes and Definitions**.

##### Offline/reference packages

- [S60v3 FP2 Developer’s Library v2.0 ZIP](https://archive.org/download/nokia_sdks_n_dev_tools/S60v3_FP2_Developers_Library_v2_0.zip)
- [S60 3rd FP2 C++ Developer’s Library JAR](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_ed_fp_2_cppDevelopers_library_v_2_0_1.0.0.jar)
- [S60 3rd FP2 API Plug-in Pack v1.3](https://archive.org/download/nokia_sdks_n_dev_tools/S60_3rd_Ed_SDK_FP2_API_Plug-In_Pack_LLA_1_3.zip)
- [Internet Archive item listing](https://archive.org/details/nokia_sdks_n_dev_tools)

#### 12. Mức độ chắc chắn

- API name, version/deprecation và public headers: **đối chiếu Quick Reference**.
- Direct API-index URL: verified pattern from the official Quick Reference target; use Quick Reference fallback if the mirror is case-sensitive.
- Library/capability: chỉ coi là confirmed khi có trong mục Build metadata hoặc đã quét SDK.
- Method signatures: header/class page trong bộ FP2 đang cài là nguồn cuối cùng.
