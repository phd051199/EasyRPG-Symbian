# Complete API usage guides 031–060

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-10-s60-quick-reference-apis-031-directory-localizer-api-md"></a>
## Directory Localizer API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/031_directory_localizer_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-031-directory-localizer-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### Directory Localizer API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Maps platform directories to localized display names. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Maps platform directories to localized display names.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cdirectorylocalizer.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cdirectorylocalizer.h>
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
findstr /s /n /i /c:"cdirectorylocalizer.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cdirectorylocalizer.h" C:\S60\devices\*.mmp
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

- [API index — Directory Localizer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Directory_Localizer_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Directory+Localizer+API%22)

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

<a id="src-10-s60-quick-reference-apis-032-document-handler-api-md"></a>
## Document Handler API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/032_document_handler_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-032-document-handler-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Document Handler API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng registered document handlers/viewers/editor của hệ thống thay vì nhúng parser/viewer riêng cho mọi MIME type.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Opens, saves or launches handlers for supported content.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CDocumentHandler`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `documenthandler.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <documenthandler.h>
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

1. Tạo handler với CEikProcess/observer phù hợp.
2. Nhận dạng MIME hoặc truyền type.
3. OpenFileL/SaveL/CopyL theo use-case.
4. Xử lý không có handler và embedded exit callback.

#### 6. Build metadata

##### Import libraries

- `commonui.lib`

```text
LIBRARY         commonui.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"documenthandler.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CDocumentHandler" C:\S60\devices\*.mmp
```

##### Capabilities

Phụ thuộc file và operation.

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
- Không giả định handler tồn tại; không mở private file mà handler khác không có quyền đọc.

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

- [API index — Document Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Document_Handler_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Document+Handler+API%22)

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

<a id="src-10-s60-quick-reference-apis-033-download-manager-engine-api-md"></a>
## Download Manager Engine API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/033_download_manager_engine_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-033-download-manager-engine-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP3
- `deprecated`: none-listed

### Download Manager Engine API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng browser Download Manager cho queue/resume/progress thay vì tự xây downloader nếu yêu cầu phù hợp.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Client interface to the browser download manager server.

- First published: **S60 2nd Ed FP3**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `RHttpDownloadMgr`, `RHttpDownload`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `downloadmgrclient.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `httpdownloadmgrcommon.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `downloadmgrclientapiext.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <downloadmgrclient.h>
#include <httpdownloadmgrcommon.h>
#include <downloadmgrclientapiext.h>
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

1. Connect manager với app UID.
2. Create/new download URL.
3. Set destination/attributes.
4. Start và observe events.
5. Persist/restore theo manager semantics.

#### 6. Build metadata

##### Import libraries

- Xác minh local SDK

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"downloadmgrclient.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"RHttpDownloadMgr" C:\S60\devices\*.mmp
```

##### Capabilities

NetworkServices + file path policy.

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
- Không đồng thời tự ghi cùng destination; xử lý disk full và user cancel.

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

- [API index — Download Manager Engine API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Download_Manager_Engine_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Download+Manager+Engine+API%22)

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

<a id="src-10-s60-quick-reference-apis-034-download-manager-ui-library-api-md"></a>
## Download Manager UI Library API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/034_download_manager_ui_library_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-034-download-manager-ui-library-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP3
- `deprecated`: none-listed

### Download Manager UI Library API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Downloads list, menu and user-interaction UI integration. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Downloads list, menu and user-interaction UI integration.

- First published: **S60 2nd Ed FP3**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cdownloadmgruibase.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `cdownloadmgruidownloadslist.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `cdownloadmgruidownloadmenu.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `cdownloadmgruilibregistry.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `cdownloadmgruiuserinteractions.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `downloadslistdlgobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cdownloadmgruibase.h>
#include <cdownloadmgruidownloadslist.h>
#include <cdownloadmgruidownloadmenu.h>
#include <cdownloadmgruilibregistry.h>
#include <cdownloadmgruiuserinteractions.h>
#include <downloadslistdlgobserver.h>
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
findstr /s /n /i /c:"cdownloadmgruibase.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cdownloadmgruibase.h" C:\S60\devices\*.mmp
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

- [API index — Download Manager UI Library API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Download_Manager_UI_Library_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Download+Manager+UI+Library+API%22)

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

<a id="src-10-s60-quick-reference-apis-035-editors-api-md"></a>
## Editors API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/035_editors_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-035-editors-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Editors API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Text, numeric, IP, secret and multi-field editor controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Text, numeric, IP, secret and multi-field editor controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknedsts.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknedstsobs.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnumed.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnumedwin.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnumseced.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknipfed.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikfpne.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikgted.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikmfne.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikrted.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikseced.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikccpu.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikedwin.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikedwob.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknmfnecommandobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `aknlocationed.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknuniteditor.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknedsts.h>
#include <aknedstsobs.h>
#include <aknnumed.h>
#include <aknnumedwin.h>
#include <aknnumseced.h>
#include <aknipfed.h>
#include <eikfpne.h>
#include <eikgted.h>
#include <eikmfne.h>
#include <eikrted.h>
#include <eikseced.h>
#include <eikccpu.h>
// + 5 header khác; chỉ include header chứa symbol thực sự dùng.
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
findstr /s /n /i /c:"aknedsts.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknedsts.h" C:\S60\devices\*.mmp
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

- [API index — Editors API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Editors_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Editors+API%22)

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

<a id="src-10-s60-quick-reference-apis-036-error-ui-api-md"></a>
## Error UI API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/036_error_ui_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-036-error-ui-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Error UI API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Displays platform-style error notes. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Displays platform-style error notes.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `errorui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <errorui.h>
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
findstr /s /n /i /c:"errorui.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"errorui.h" C:\S60\devices\*.mmp
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

- [API index — Error UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Error_UI_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Error+UI+API%22)

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

<a id="src-10-s60-quick-reference-apis-037-exif-api-md"></a>
## Exif API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/037_exif_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-037-exif-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Exif API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng EXIF reader/modifier thay vì tự parse JPEG APP1/TIFF byte order.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Reads, creates and modifies EXIF metadata in JPEG files.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CExifRead`, `CExifModify`, `CExifTag`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `exifread.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `exifmodify.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `exiftag.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <exifread.h>
#include <exifmodify.h>
#include <exiftag.h>
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

1. Load JPEG data/file theo API.
2. Read tag bằng IFD/tag ID.
3. Validate type/count.
4. Modify/create metadata.
5. Write output atomically.

#### 6. Build metadata

##### Import libraries

- Xác minh trong SDK/class page

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"exifread.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CExifRead" C:\S60\devices\*.mmp
```

##### Capabilities

File path/data policy.

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
- Không sửa byte offset trực tiếp; thumbnail/endianness dễ hỏng.

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

- [API index — Exif API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Exif_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Exif+API%22)

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

<a id="src-10-s60-quick-reference-apis-038-favourites-engine-api-md"></a>
## Favourites Engine API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/038_favourites_engine_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-038-favourites-engine-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Favourites Engine API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy browser favourites and bookmark database access. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy browser favourites and bookmark database access.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `favouritesdb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouritesfile.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouritesdbincremental.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouritesdbnotifier.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouritesdbobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `activefavouritesdbnotifier.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouritesitem.h` | Data/value object or record definitions used by the API. |
| `favouriteslimits.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouriteswapap.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `favouritesitemlist.h` | Data/value object or record definitions used by the API. |
| `favouritesitemdata.h` | Data/value object or record definitions used by the API. |
| `favouritessession.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `favouriteshandle.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <favouritesdb.h>
#include <favouritesfile.h>
#include <favouritesdbincremental.h>
#include <favouritesdbnotifier.h>
#include <favouritesdbobserver.h>
#include <activefavouritesdbnotifier.h>
#include <favouritesitem.h>
#include <favouriteslimits.h>
#include <favouriteswapap.h>
#include <favouritesitemlist.h>
#include <favouritesitemdata.h>
#include <favouritessession.h>
// + 1 header khác; chỉ include header chứa symbol thực sự dùng.
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
findstr /s /n /i /c:"favouritesdb.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"favouritesdb.h" C:\S60\devices\*.mmp
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

- [API index — Favourites Engine API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Favourites_Engine_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Favourites+Engine+API%22)

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

<a id="src-10-s60-quick-reference-apis-039-feature-discovery-api-md"></a>
## Feature Discovery API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/039_feature_discovery_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-039-feature-discovery-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP3
- `deprecated`: none-listed

### Feature Discovery API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng feature flags trước khi bật chức năng phụ thuộc firmware/phần cứng; đừng suy luận từ model name hoặc version string.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Checks whether a platform feature is supported.

- First published: **S60 2nd Ed FP3**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CFeatureDiscovery`, `FeatureManager`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `featdiscovery.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `featureinfo.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <featdiscovery.h>
#include <featureinfo.h>
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

1. Chọn synchronous helper hoặc initialize FeatureManager.
2. Query feature UID/ID.
3. Giữ fallback path nếu false hoặc KErrNotFound.
4. Balance InitializeLibL/UnInitializeLib nếu dùng static manager.

#### 6. Build metadata

##### Import libraries

- `featdiscovery.lib`
- featmgr.lib (tùy interface dùng)

```text
LIBRARY         featdiscovery.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"featdiscovery.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CFeatureDiscovery" C:\S60\devices\*.mmp
```

##### Capabilities

Thường None; kiểm tra exact member.

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
- Không cache mãi qua firmware update; không dùng version check thay feature check khi feature flag tồn tại.

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

- [API index — Feature Discovery API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Feature_Discovery_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Feature+Discovery+API%22)

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

<a id="src-10-s60-quick-reference-apis-040-flash-viewer-framework-api-md"></a>
## Flash Viewer Framework API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/040_flash_viewer_framework_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-040-flash-viewer-framework-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP1
- `deprecated`: none-listed

### Flash Viewer Framework API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Launches the Flash viewer for SWF content. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Launches the Flash viewer for SWF content.

- First published: **S60 3rd Ed FP1**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `flash_ui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <flash_ui.h>
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
findstr /s /n /i /c:"flash_ui.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"flash_ui.h" C:\S60\devices\*.mmp
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

- [API index — Flash Viewer Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Flash_Viewer_Framework_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Flash+Viewer+Framework+API%22)

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

<a id="src-10-s60-quick-reference-apis-041-fonts-api-md"></a>
## Fonts API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/041_fonts_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-041-fonts-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Fonts API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: S60 font and text-decoration utilities. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

S60 font and text-decoration utilities.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknlayoutfont.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akntextdecorationmetrics.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknfontaccess.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknlayoutfont.h>
#include <akntextdecorationmetrics.h>
#include <aknfontaccess.h>
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
findstr /s /n /i /c:"aknlayoutfont.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknlayoutfont.h" C:\S60\devices\*.mmp
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

- [API index — Fonts API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Fonts_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Fonts+API%22)

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

<a id="src-10-s60-quick-reference-apis-042-form-api-md"></a>
## Form API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/042_form_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-042-form-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Form API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Avkon form component. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Avkon form component.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknform.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknform.h>
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
findstr /s /n /i /c:"aknform.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknform.h" C:\S60\devices\*.mmp
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

- [API index — Form API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Form_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Form+API%22)

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

<a id="src-10-s60-quick-reference-apis-043-grids-api-md"></a>
## Grids API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/043_grids_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-043-grids-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Grids API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Grid controls, models and views. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Grid controls, models and views.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akngmsstylegrid.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akngrid.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akngridview.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akngridm.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akncolourselectiongrid.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akngmsstylegrid.h>
#include <akngrid.h>
#include <akngridview.h>
#include <akngridm.h>
#include <akncolourselectiongrid.h>
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
findstr /s /n /i /c:"akngmsstylegrid.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akngmsstylegrid.h" C:\S60\devices\*.mmp
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

- [API index — Grids API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Grids_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Grids+API%22)

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

<a id="src-10-s60-quick-reference-apis-044-hotkeys-api-md"></a>
## Hotkeys API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/044_hotkeys_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-044-hotkeys-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Hotkeys API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Hotkey registration and handling. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Hotkey registration and handling.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikhkeyt.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikhkeyt.h>
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
findstr /s /n /i /c:"eikhkeyt.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikhkeyt.h" C:\S60\devices\*.mmp
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

- [API index — Hotkeys API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Hotkeys_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Hotkeys+API%22)

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

<a id="src-10-s60-quick-reference-apis-045-instant-messaging-api-md"></a>
## Instant Messaging API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/045_instant_messaging_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-045-instant-messaging-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Instant Messaging API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: ECom-based client interfaces for instant-messaging services. Dùng khi cần mở rộng platform qua ECom/provider/observer contract thay vì hard-code implementation vào app.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

ECom-based client interfaces for instant-messaging services.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `imconnection.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `imclient.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `imerrors.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <imconnection.h>
#include <imclient.h>
#include <imerrors.h>
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
findstr /s /n /i /c:"imconnection.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"imconnection.h" C:\S60\devices\*.mmp
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

- [API index — Instant Messaging API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Instant_Messaging_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Instant+Messaging+API%22)

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

<a id="src-10-s60-quick-reference-apis-046-im-application-launch-api-md"></a>
## IM Application Launch API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/046_im_application_launch_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-046-im-application-launch-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### IM Application Launch API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: ECom plug-in contract for launching an IM application on events. Dùng khi cần mở rộng platform qua ECom/provider/observer contract thay vì hard-code implementation vào app.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

ECom plug-in contract for launching an IM application on events.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `imlauncherplugin.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <imlauncherplugin.h>
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
findstr /s /n /i /c:"imlauncherplugin.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"imlauncherplugin.h" C:\S60\devices\*.mmp
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

- [API index — IM Application Launch API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/IM_Application_Launch_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22IM+Application+Launch+API%22)

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

<a id="src-10-s60-quick-reference-apis-047-images-api-md"></a>
## Images API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/047_images_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-047-images-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Images API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Basic UI image control. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Basic UI image control.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikimage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikimage.h>
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
findstr /s /n /i /c:"eikimage.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikimage.h" C:\S60\devices\*.mmp
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

- [API index — Images API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Images_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Images+API%22)

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

<a id="src-10-s60-quick-reference-apis-048-indicators-api-md"></a>
## Indicators API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/048_indicators_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-048-indicators-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Indicators API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Status and edit-state indicator controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Status and edit-state indicator controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknindicatorcontainer.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akneditstateindicator.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknindicatorcontainer.h>
#include <akneditstateindicator.h>
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
findstr /s /n /i /c:"aknindicatorcontainer.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknindicatorcontainer.h" C:\S60\devices\*.mmp
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

- [API index — Indicators API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Indicators_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Indicators+API%22)

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

<a id="src-10-s60-quick-reference-apis-049-info-popup-note-api-md"></a>
## Info Popup Note API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/049_info_popup_note_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-049-info-popup-note-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Info Popup Note API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Transient information popup note. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Transient information popup note.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akninfopopupnotecontroller.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akninfopopupnotecontroller.h>
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
findstr /s /n /i /c:"akninfopopupnotecontroller.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akninfopopupnotecontroller.h" C:\S60\devices\*.mmp
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

- [API index — Info Popup Note API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Info_Popup_Note_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Info+Popup+Note+API%22)

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

<a id="src-10-s60-quick-reference-apis-050-information-preview-popup-api-md"></a>
## Information Preview Popup API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/050_information_preview_popup_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-050-information-preview-popup-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### Information Preview Popup API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Preview popup controller and observer interfaces. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Preview popup controller and observer interfaces.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknpreviewpopupcontentprovider.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpreviewpopupcontroller.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknpreviewpopupobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknpreviewpopupcontentprovider.h>
#include <aknpreviewpopupcontroller.h>
#include <aknpreviewpopupobserver.h>
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
findstr /s /n /i /c:"aknpreviewpopupcontentprovider.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknpreviewpopupcontentprovider.h" C:\S60\devices\*.mmp
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

- [API index — Information Preview Popup API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Information_Preview_Popup_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Information+Preview+Popup+API%22)

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

<a id="src-10-s60-quick-reference-apis-051-initialization-api-md"></a>
## Initialization API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/051_initialization_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-051-initialization-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Initialization API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy UI control-library initialization. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy UI control-library initialization.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikctlib.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikcolib.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikctlib.h>
#include <eikcolib.h>
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
findstr /s /n /i /c:"eikctlib.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikctlib.h" C:\S60\devices\*.mmp
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

- [API index — Initialization API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Initialization_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Initialization+API%22)

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

<a id="src-10-s60-quick-reference-apis-052-input-frame-api-md"></a>
## Input Frame API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/052_input_frame_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-052-input-frame-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Input Frame API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Input-frame control. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Input-frame control.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akninfrm.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akninfrm.h>
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
findstr /s /n /i /c:"akninfrm.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akninfrm.h" C:\S60\devices\*.mmp
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

- [API index — Input Frame API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Input_Frame_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Input+Frame+API%22)

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

<a id="src-10-s60-quick-reference-apis-053-input-language-info-api-md"></a>
## Input Language Info API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/053_input_language_info_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-053-input-language-info-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Input Language Info API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Enumerates available input languages. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Enumerates available input languages.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akninputlanguageinfo.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akninputlanguageinfo.h>
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
findstr /s /n /i /c:"akninputlanguageinfo.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akninputlanguageinfo.h" C:\S60\devices\*.mmp
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

- [API index — Input Language Info API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Input_Language_Info_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Input+Language+Info+API%22)

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

<a id="src-10-s60-quick-reference-apis-054-intermediate-state-api-md"></a>
## Intermediate State API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/054_intermediate_state_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-054-intermediate-state-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Intermediate State API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Intermediate-state support for controls. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Intermediate-state support for controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknintermediate.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknintermediate.h>
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
findstr /s /n /i /c:"aknintermediate.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknintermediate.h" C:\S60\devices\*.mmp
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

- [API index — Intermediate State API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Intermediate_State_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Intermediate+State+API%22)

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

<a id="src-10-s60-quick-reference-apis-055-keylock-api-md"></a>
## Keylock API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/055_keylock_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-055-keylock-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Keylock API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Keyguard and keylock utilities. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Keyguard and keylock utilities.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknkeylock.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknkeylock.h>
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
findstr /s /n /i /c:"aknkeylock.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknkeylock.h" C:\S60\devices\*.mmp
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

- [API index — Keylock API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Keylock_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Keylock+API%22)

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

<a id="src-10-s60-quick-reference-apis-056-labels-api-md"></a>
## Labels API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/056_labels_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-056-labels-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Labels API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Label controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Label controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikfnlab.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklabel.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikfnlab.h>
#include <eiklabel.h>
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
findstr /s /n /i /c:"eikfnlab.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikfnlab.h" C:\S60\devices\*.mmp
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

- [API index — Labels API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Labels_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Labels+API%22)

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

<a id="src-10-s60-quick-reference-apis-057-landmarks-api-md"></a>
## Landmarks API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/057_landmarks_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-057-landmarks-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Landmarks API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng Landmarks database/model thay vì tự tạo schema địa điểm riêng nếu cần tương tác dữ liệu platform.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Creates, reads, updates and categorizes landmarks.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CPosLandmarkDatabase`, `CPosLandmark`, `CPosLandmarkCategory`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `epos_landmarks.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlandmark.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlandmarkcategory.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlandmarkdatabase.h` | Data/value object or record definitions used by the API. |
| `epos_cposlmcategorymanager.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `epos_cposlmitemiterator.h` | Data/value object or record definitions used by the API. |
| `epos_cposlandmarkparser.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlandmarkencoder.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmoperation.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmpartialreadparameters.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_tposlmsortpref.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_poslmcategoryserialization.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_poslandmarkserialization.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <epos_landmarks.h>
#include <epos_cposlandmark.h>
#include <epos_cposlandmarkcategory.h>
#include <epos_cposlandmarkdatabase.h>
#include <epos_cposlmcategorymanager.h>
#include <epos_cposlmitemiterator.h>
#include <epos_cposlandmarkparser.h>
#include <epos_cposlandmarkencoder.h>
#include <epos_cposlmoperation.h>
#include <epos_cposlmpartialreadparameters.h>
#include <epos_tposlmsortpref.h>
#include <epos_poslmcategoryserialization.h>
// + 1 header khác; chỉ include header chứa symbol thực sự dùng.
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

1. Open database.
2. Tạo/read landmark.
3. Set coordinates/name/category.
4. Add/Update/Delete bằng operation phù hợp.
5. Thực thi CPosLmOperation và cleanup.

#### 6. Build metadata

##### Import libraries

- `lbslandmarks.lib`

```text
LIBRARY         lbslandmarks.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"epos_landmarks.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CPosLandmarkDatabase" C:\S60\devices\*.mmp
```

##### Capabilities

Location/data policy tùy database.

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
- Không block UI khi operation dài; không giả định default database writable.

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

- [API index — Landmarks API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Landmarks+API%22)

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

<a id="src-10-s60-quick-reference-apis-058-landmarks-database-management-api-md"></a>
## Landmarks Database Management API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/058_landmarks_database_management_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-058-landmarks-database-management-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Landmarks Database Management API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Creates, lists, copies, moves and deletes landmark databases. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Creates, lists, copies, moves and deletes landmark databases.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `epos_cposlmdatabasemanager.h` | Data/value object or record definitions used by the API. |
| `epos_hposlmdatabaseinfo.h` | Data/value object or record definitions used by the API. |
| `epos_tposlmdatabasesettings.h` | Data/value object or record definitions used by the API. |
| `epos_tposlmdatabaseevent.h` | Data/value object or record definitions used by the API. |
| `epos_cposlmdatabasemanagerpluginbase.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <epos_cposlmdatabasemanager.h>
#include <epos_hposlmdatabaseinfo.h>
#include <epos_tposlmdatabasesettings.h>
#include <epos_tposlmdatabaseevent.h>
#include <epos_cposlmdatabasemanagerpluginbase.h>
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
findstr /s /n /i /c:"epos_cposlmdatabasemanager.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"epos_cposlmdatabasemanager.h" C:\S60\devices\*.mmp
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

- [API index — Landmarks Database Management API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_Database_Management_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Landmarks+Database+Management+API%22)

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

<a id="src-10-s60-quick-reference-apis-059-landmarks-search-api-md"></a>
## Landmarks Search API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/059_landmarks_search_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-059-landmarks-search-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Landmarks Search API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng search criteria có sẵn (text/category/area/nearest/composite) thay vì load toàn bộ landmark rồi lọc thủ công.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Searches landmark databases by text, category, area or distance.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CPosLandmarkSearch`, `CPosLmSearchCriteria`, `CPosLmNearestCriteria`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `epos_cposlandmarksearch.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmareacriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmcategorycriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmcatnamecriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmcompositecriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmidlistcriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmnearestcriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmsearchcriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmtextcriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `epos_cposlmdisplaydata.h` | Data/value object or record definitions used by the API. |
| `epos_cposlmdisplayitem.h` | Data/value object or record definitions used by the API. |
| `epos_cposlmmultidbsearch.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <epos_cposlandmarksearch.h>
#include <epos_cposlmareacriteria.h>
#include <epos_cposlmcategorycriteria.h>
#include <epos_cposlmcatnamecriteria.h>
#include <epos_cposlmcompositecriteria.h>
#include <epos_cposlmidlistcriteria.h>
#include <epos_cposlmnearestcriteria.h>
#include <epos_cposlmsearchcriteria.h>
#include <epos_cposlmtextcriteria.h>
#include <epos_cposlmdisplaydata.h>
#include <epos_cposlmdisplayitem.h>
#include <epos_cposlmmultidbsearch.h>
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

1. Open search on DB.
2. Construct criteria.
3. StartLandmarkSearchL.
4. Execute async operation.
5. Read iterator/IDs/display data.

#### 6. Build metadata

##### Import libraries

- `lbslandmarks.lib`

```text
LIBRARY         lbslandmarks.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"epos_cposlandmarksearch.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CPosLandmarkSearch" C:\S60\devices\*.mmp
```

##### Capabilities

Location/data policy.

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
- Không quét full DB trên UI thread.

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

- [API index — Landmarks Search API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_Search_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Landmarks+Search+API%22)

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

<a id="src-10-s60-quick-reference-apis-060-landmarks-ui-add-edit-api-md"></a>
## Landmarks UI Add Edit API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/060_landmarks_ui_add_edit_api.md`
- Consolidated reference: `22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-060-landmarks-ui-add-edit-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Landmarks UI Add Edit API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Platform dialog for adding or editing landmarks. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Platform dialog for adding or editing landmarks.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `clmkeditordlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <clmkeditordlg.h>
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
findstr /s /n /i /c:"clmkeditordlg.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"clmkeditordlg.h" C:\S60\devices\*.mmp
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

- [API index — Landmarks UI Add Edit API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_UI_Add_Edit_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Landmarks+UI+Add+Edit+API%22)

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
