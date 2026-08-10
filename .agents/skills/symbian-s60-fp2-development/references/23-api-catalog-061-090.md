# Complete API usage guides 061–090

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-10-s60-quick-reference-apis-061-landmarks-ui-selector-api-md"></a>
## Landmarks UI Selector API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/061_landmarks_ui_selector_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-061-landmarks-ui-selector-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Landmarks UI Selector API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Platform dialogs for selecting landmarks or categories. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Platform dialogs for selecting landmarks or categories.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `clmkcategoryselectordlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `clmklandmarkselectordlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `tlmkitemiddbcombiinfo.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <clmkcategoryselectordlg.h>
#include <clmklandmarkselectordlg.h>
#include <tlmkitemiddbcombiinfo.h>
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
findstr /s /n /i /c:"clmkcategoryselectordlg.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"clmkcategoryselectordlg.h" C:\S60\devices\*.mmp
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

- [API index — Landmarks UI Selector API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_UI_Selector_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Landmarks+UI+Selector+API%22)

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

<a id="src-10-s60-quick-reference-apis-062-light-api-md"></a>
## Light API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/062_light_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-062-light-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Light API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng HWRM Light thay vì điều khiển hardware trực tiếp.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Controls supported device lights through HWRM.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CHWRMLight`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `hwrmlight.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <hwrmlight.h>
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

1. NewL.
2. Chọn supported target.
3. LightOnL/LightOffL/LightBlinkL.
4. Release control và xử lý status.
5. Fallback khi target unsupported.

#### 6. Build metadata

##### Import libraries

- `hwrmlightclient.lib`

```text
LIBRARY         hwrmlightclient.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"hwrmlight.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CHWRMLight" C:\S60\devices\*.mmp
```

##### Capabilities

Device/operation policy; kiểm tra class member.

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
- Không giữ light vô thời hạn; tôn trọng power/profile.

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

- [API index — Light API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Light_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Light+API%22)

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

<a id="src-10-s60-quick-reference-apis-063-lists-api-md"></a>
## Lists API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/063_lists_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Lists API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Avkon list boxes, models, views and selection lists. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Avkon list boxes, models, views and selection lists.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknlistboxlayoutdecorator.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknlistboxsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknlists.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknselectionlist.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikclb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikclbd.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikfrlb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikfrlbd.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikslb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbd.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbed.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbi.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbm.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbo.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbv.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiklbx.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiktxlbm.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiktxlbx.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknlistboxlayoutdecorator.h>
#include <aknlistboxsettingpage.h>
#include <aknlists.h>
#include <aknselectionlist.h>
#include <eikclb.h>
#include <eikclbd.h>
#include <eikfrlb.h>
#include <eikfrlbd.h>
#include <eikslb.h>
#include <eiklbd.h>
#include <eiklbed.h>
#include <eiklbi.h>
// + 6 header khác; chỉ include header chứa symbol thực sự dùng.
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
findstr /s /n /i /c:"aknlistboxlayoutdecorator.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknlistboxlayoutdecorator.h" C:\S60\devices\*.mmp
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

- [API index — Lists API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Lists_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Lists+API%22)

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

<a id="src-10-s60-quick-reference-apis-064-location-acquisition-api-md"></a>
## Location Acquisition API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/064_location_acquisition_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP2
- `deprecated`: none-listed

### Location Acquisition API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng Location Server thay vì tự nói chuyện trực tiếp với GPS. API hỗ trợ chọn module, yêu cầu position bất đồng bộ, satellite info và update options.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Position-server clients, positioning modules and position data types.

- First published: **S60 2nd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `RPositionServer`, `RPositioner`, `TPositionInfo`, `TPositionSatelliteInfo`, `TPositionUpdateOptions`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `lbs.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbsclasstypes.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbscommon.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `lbscriteria.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbserrors.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbsfieldids.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbsfields.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbsipc.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbsposition.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbspositioninfo.h` | Data/value object or record definitions used by the API. |
| `lbsptrholder.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbsrequestor.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `lbssatellite.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <lbs.h>
#include <lbsclasstypes.h>
#include <lbscommon.h>
#include <lbscriteria.h>
#include <lbserrors.h>
#include <lbsfieldids.h>
#include <lbsfields.h>
#include <lbsipc.h>
#include <lbsposition.h>
#include <lbspositioninfo.h>
#include <lbsptrholder.h>
#include <lbsrequestor.h>
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

1. Connect RPositionServer.
2. Open RPositioner (default hoặc module cụ thể).
3. SetRequestor() đúng danh tính ứng dụng.
4. SetUpdateOptions() nếu cần timeout/max age/partial updates.
5. NotifyPositionUpdate() bằng Active Object.
6. CancelRequest() trước khi Close().

#### 6. Build metadata

##### Import libraries

- `lbs.lib`

```text
LIBRARY         lbs.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"lbs.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"RPositionServer" C:\S60\devices\*.mmp
```

##### Capabilities

Location; một số operation/requestor policy còn phụ thuộc firmware và signing.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
User::LeaveIfError(iPosServer.Connect());
User::LeaveIfError(iPositioner.Open(iPosServer));
User::LeaveIfError(iPositioner.SetRequestor(
    CRequestor::ERequestorService,
    CRequestor::EFormatApplication,
    _L("MyApp")));
iPositioner.NotifyPositionUpdate(iPositionInfo, iStatus);
SetActive();
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
- Không block UI bằng wait loop; không bỏ SetRequestor; không giả định GPS có fix trong nhà; không dùng satellite fields khi module không hỗ trợ.

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

- [API index — Location Acquisition API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Location_Acquisition_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Location+Acquisition+API%22)

##### API-specific subpages

- [Location_Acquisition_API2.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Location_Acquisition_API2.html)
- [Location_Acquisition_API3.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Location_Acquisition_API3.html)
- [Location_Acquisition_API4.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Location_Acquisition_API4.html)
- [Location_Acquisition_APIClassesIndexPage.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Location_Acquisition_APIClassesIndexPage.html)

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

<a id="src-10-s60-quick-reference-apis-065-mms-client-mtm-api-md"></a>
## MMS Client MTM API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/065_mms_client_mtm_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-065-mms-client-mtm-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### MMS Client MTM API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Client MTM interfaces for MMS. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Client MTM interfaces for MMS.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `mmsclient.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mmsconst.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `mmserrors.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <mmsclient.h>
#include <mmsconst.h>
#include <mmserrors.h>
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
findstr /s /n /i /c:"mmsclient.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"mmsclient.h" C:\S60\devices\*.mmp
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

- [API index — MMS Client MTM API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/MMS_Client_MTM_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22MMS+Client+MTM+API%22)

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

<a id="src-10-s60-quick-reference-apis-066-media-fetch-api-md"></a>
## Media Fetch API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/066_media_fetch_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-066-media-fetch-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Media Fetch API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Platform media-file picker. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Platform media-file picker.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `mgfetch.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mmgfetchverifier.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mmgfetchcanceler.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mediafiletypes.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <mgfetch.h>
#include <mmgfetchverifier.h>
#include <mmgfetchcanceler.h>
#include <mediafiletypes.hrh>
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
findstr /s /n /i /c:"mgfetch.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"mgfetch.h" C:\S60\devices\*.mmp
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

- [API index — Media Fetch API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Media_Fetch_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Media+Fetch+API%22)

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

<a id="src-10-s60-quick-reference-apis-067-misc-controls-api-md"></a>
## Misc Controls API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/067_misc_controls_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-067-misc-controls-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Misc Controls API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Miscellaneous UI containers and controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Miscellaneous UI containers and controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikcapc.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikcapca.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikdpobs.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikmover.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikconso.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikctgrp.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikkwin.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiktbar.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikcapc.h>
#include <eikcapca.h>
#include <eikdpobs.h>
#include <eikmover.h>
#include <eikconso.h>
#include <eikctgrp.h>
#include <eikkwin.h>
#include <eiktbar.h>
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
findstr /s /n /i /c:"eikcapc.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikcapc.h" C:\S60\devices\*.mmp
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

- [API index — Misc Controls API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Misc_Controls_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Misc+Controls+API%22)

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

<a id="src-10-s60-quick-reference-apis-068-navigation-pane-api-md"></a>
## Navigation Pane API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/068_navigation_pane_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Navigation Pane API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Navigation pane decorators, labels and observers. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Navigation pane decorators, labels and observers.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknnavi.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnavide.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnavilabel.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnaviobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `aknnavidecoratorobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknnavi.h>
#include <aknnavide.h>
#include <aknnavilabel.h>
#include <aknnaviobserver.h>
#include <aknnavidecoratorobserver.h>
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
findstr /s /n /i /c:"aknnavi.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknnavi.h" C:\S60\devices\*.mmp
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

- [API index — Navigation Pane API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Navigation_Pane_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Navigation+Pane+API%22)

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

<a id="src-10-s60-quick-reference-apis-069-notepad-library-api-md"></a>
## Notepad Library API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/069_notepad_library_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-069-notepad-library-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Notepad Library API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Stores and edits notes, templates and text files. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Stores and edits notes, templates and text files.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `npdapi.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <npdapi.h>
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
findstr /s /n /i /c:"npdapi.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"npdapi.h" C:\S60\devices\*.mmp
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

- [API index — Notepad Library API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Notepad_Library_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Notepad+Library+API%22)

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

<a id="src-10-s60-quick-reference-apis-070-notes-api-md"></a>
## Notes API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/070_notes_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-070-notes-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Notes API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Information, wait and progress notes/dialogs. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Information, wait and progress notes/dialogs.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknnotecontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknnotedialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknnotewrappers.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknprogressdialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknprogresstimer.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknstaticnotedialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknwaitdialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknwaitnotewrapper.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikinfo.h` | Data/value object or record definitions used by the API. |
| `eikprogi.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknnotecontrol.h>
#include <aknnotedialog.h>
#include <aknnotewrappers.h>
#include <aknprogressdialog.h>
#include <aknprogresstimer.h>
#include <aknstaticnotedialog.h>
#include <aknwaitdialog.h>
#include <aknwaitnotewrapper.h>
#include <eikinfo.h>
#include <eikprogi.h>
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
findstr /s /n /i /c:"aknnotecontrol.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknnotecontrol.h" C:\S60\devices\*.mmp
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

- [API index — Notes API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Notes_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Notes+API%22)

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

<a id="src-10-s60-quick-reference-apis-071-notifiers-api-md"></a>
## Notifiers API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/071_notifiers_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Notifiers API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Global queries, notes and notifier integration. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Global queries, notes and notifier integration.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknglobalconfirmationquery.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akngloballistquery.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknglobalmsgquery.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknglobalnote.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknglobalprogressdialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknnotify.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknnotifystd.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupnotify.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknglobalconfirmationquery.h>
#include <akngloballistquery.h>
#include <aknglobalmsgquery.h>
#include <aknglobalnote.h>
#include <aknglobalprogressdialog.h>
#include <aknnotify.h>
#include <aknnotifystd.h>
#include <aknpopupnotify.h>
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
findstr /s /n /i /c:"aknglobalconfirmationquery.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknglobalconfirmationquery.h" C:\S60\devices\*.mmp
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

- [API index — Notifiers API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Notifiers_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Notifiers+API%22)

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

<a id="src-10-s60-quick-reference-apis-072-oma-drm-caf-agent-api-md"></a>
## OMA DRM CAF Agent API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/072_oma_drm_caf_agent_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-072-oma-drm-caf-agent-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### OMA DRM CAF Agent API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: OMA DRM CAF-agent-specific operations. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

OMA DRM CAF-agent-specific operations.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `oma2agent.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <oma2agent.h>
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
findstr /s /n /i /c:"oma2agent.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"oma2agent.h" C:\S60\devices\*.mmp
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

- [API index — OMA DRM CAF Agent API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/OMA_DRM_CAF_Agent_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22OMA+DRM+CAF+Agent+API%22)

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

<a id="src-10-s60-quick-reference-apis-073-new-message-notification-api-md"></a>
## New Message Notification API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/073_new_message_notification_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-073-new-message-notification-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### New Message Notification API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Notifies the platform new-message component. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Notifies the platform new-message component.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `mncnnotification.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <mncnnotification.h>
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
findstr /s /n /i /c:"mncnnotification.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"mncnnotification.h" C:\S60\devices\*.mmp
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

- [API index — New Message Notification API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/New_Message_Notification_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22New+Message+Notification+API%22)

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

<a id="src-10-s60-quick-reference-apis-074-optical-character-recognition-api-md"></a>
## Optical Character Recognition API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/074_optical_character_recognition_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-074-optical-character-recognition-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP1
- `deprecated`: none-listed

### Optical Character Recognition API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: OCR client types and server access. Dùng khi platform đã có engine/utility/codec/DRM/browser/download component; tránh tự viết decoder, metadata parser, player state machine hoặc download persistence.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

OCR client types and server access.

- First published: **S60 3rd Ed FP1**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `ocrcommon.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `ocrsrv.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <ocrcommon.h>
#include <ocrsrv.h>
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
findstr /s /n /i /c:"ocrcommon.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"ocrcommon.h" C:\S60\devices\*.mmp
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

- [API index — Optical Character Recognition API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Optical_Character_Recognition_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Optical+Character+Recognition+API%22)

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

<a id="src-10-s60-quick-reference-apis-075-options-menu-api-md"></a>
## Options Menu API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/075_options_menu_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-075-options-menu-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Options Menu API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Menu bar and menu pane controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Menu bar and menu pane controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikmenub.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikmenup.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikmenub.h>
#include <eikmenup.h>
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
findstr /s /n /i /c:"eikmenub.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikmenub.h" C:\S60\devices\*.mmp
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

- [API index — Options Menu API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Options_Menu_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Options+Menu+API%22)

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

<a id="src-10-s60-quick-reference-apis-076-phone-parser-api-md"></a>
## Phone Parser API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/076_phone_parser_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-076-phone-parser-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Phone Parser API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Parses and validates telephone numbers. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Parses and validates telephone numbers.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `commonphoneparser.h` | Shared constants, enums, UIDs, error codes or common value types. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <commonphoneparser.h>
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
findstr /s /n /i /c:"commonphoneparser.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"commonphoneparser.h" C:\S60\devices\*.mmp
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

- [API index — Phone Parser API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phone_Parser_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phone+Parser+API%22)

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

<a id="src-10-s60-quick-reference-apis-077-phonebook-data-add-edit-ui-api-md"></a>
## Phonebook Data Add Edit UI API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/077_phonebook_data_add_edit_ui_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-077-phonebook-data-add-edit-ui-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 5th Ed

### Phonebook Data Add Edit UI API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy Phonebook dialogs for adding and editing contact data. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy Phonebook dialogs for adding and editing contact data.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cpbkcontacteditordlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkdatasaveappui.h` | Data/value object or record definitions used by the API. |
| `pbkdatasaveappui.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `mpbkeditoroktoexitcallback.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cpbkcontacteditordlg.h>
#include <cpbkdatasaveappui.h>
#include <pbkdatasaveappui.hrh>
#include <mpbkeditoroktoexitcallback.h>
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
findstr /s /n /i /c:"cpbkcontacteditordlg.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cpbkcontacteditordlg.h" C:\S60\devices\*.mmp
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

- [API index — Phonebook Data Add Edit UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_Data_Add_Edit_UI_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phonebook+Data+Add+Edit+UI+API%22)

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

<a id="src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md"></a>
## Phonebook Data Management API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/078_phonebook_data_management_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 5th Ed

### Phonebook Data Management API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: S60 wrapper around the Symbian Contact Model. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

S60 wrapper around the Symbian Contact Model.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cpbkcontactchangenotifier.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkcontactengine.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `cpbkcontactitem.h` | Data/value object or record definitions used by the API. |
| `cpbkcontactiter.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkfieldinfo.h` | Data/value object or record definitions used by the API. |
| `cpbkfieldsinfo.h` | Data/value object or record definitions used by the API. |
| `cpbkidlefinder.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mpbkcontactdbobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mpbkfielddata.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `pbkfields.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `pbkiconid.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `tpbkcontactitemfield.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cpbkcontactchangenotifier.h>
#include <cpbkcontactengine.h>
#include <cpbkcontactitem.h>
#include <cpbkcontactiter.h>
#include <cpbkfieldinfo.h>
#include <cpbkfieldsinfo.h>
#include <cpbkidlefinder.h>
#include <mpbkcontactdbobserver.h>
#include <mpbkfielddata.h>
#include <pbkfields.hrh>
#include <pbkiconid.hrh>
#include <tpbkcontactitemfield.h>
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
findstr /s /n /i /c:"cpbkcontactchangenotifier.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cpbkcontactchangenotifier.h" C:\S60\devices\*.mmp
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

- [API index — Phonebook Data Management API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_Data_Management_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phonebook+Data+Management+API%22)

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

<a id="src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md"></a>
## Phonebook Fetch UI API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/079_phonebook_fetch_ui_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 5th Ed

### Phonebook Fetch UI API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy Phonebook pickers for contacts, numbers and addresses. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy Phonebook pickers for contacts, numbers and addresses.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cpbkaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkemailaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkmemoryentryadditemdlg.h` | Data/value object or record definitions used by the API. |
| `cpbkmmsaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkmultipleentryfetchdlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkphonenumberselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkphonenumberselectbase.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkselectfielddlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbksmsaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mpbkfetchcallbacks.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `cpbksingleentryfetchdlg.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbksingleitemfetchdlg.h` | Data/value object or record definitions used by the API. |
| `cpbkemailoversmsaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkpocaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mpbkfetchdlgselection.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `cpbkvideonumberselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cpbkvoipaddressselect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cpbkaddressselect.h>
#include <cpbkemailaddressselect.h>
#include <cpbkmemoryentryadditemdlg.h>
#include <cpbkmmsaddressselect.h>
#include <cpbkmultipleentryfetchdlg.h>
#include <cpbkphonenumberselect.h>
#include <cpbkphonenumberselectbase.h>
#include <cpbkselectfielddlg.h>
#include <cpbksmsaddressselect.h>
#include <mpbkfetchcallbacks.h>
#include <cpbksingleentryfetchdlg.h>
#include <cpbksingleitemfetchdlg.h>
// + 5 header khác; chỉ include header chứa symbol thực sự dùng.
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
findstr /s /n /i /c:"cpbkaddressselect.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cpbkaddressselect.h" C:\S60\devices\*.mmp
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

- [API index — Phonebook Fetch UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_Fetch_UI_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phonebook+Fetch+UI+API%22)

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

<a id="src-10-s60-quick-reference-apis-080-phonebook-thumbnail-operation-api-md"></a>
## Phonebook Thumbnail Operation API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/080_phonebook_thumbnail_operation_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-080-phonebook-thumbnail-operation-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP3
- `deprecated`: S60 5th Ed

### Phonebook Thumbnail Operation API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Observer interfaces for Phonebook thumbnail operations. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Observer interfaces for Phonebook thumbnail operations.

- First published: **S60 2nd Ed FP3**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `mpbkthumbnailoperationobservers.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <mpbkthumbnailoperationobservers.h>
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
findstr /s /n /i /c:"mpbkthumbnailoperationobservers.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"mpbkthumbnailoperationobservers.h" C:\S60\devices\*.mmp
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

- [API index — Phonebook Thumbnail Operation API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_Thumbnail_Operation_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phonebook+Thumbnail+Operation+API%22)

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

<a id="src-10-s60-quick-reference-apis-081-phonebook-ui-utilities-api-md"></a>
## Phonebook UI Utilities API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/081_phonebook_ui_utilities_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-081-phonebook-ui-utilities-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 5th Ed

### Phonebook UI Utilities API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy Phonebook UI helper types. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy Phonebook UI helper types.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `cpbkviewstate.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `pbkiconinfo.h` | Data/value object or record definitions used by the API. |
| `rpbkviewresourcefile.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <cpbkviewstate.h>
#include <pbkiconinfo.h>
#include <rpbkviewresourcefile.h>
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
findstr /s /n /i /c:"cpbkviewstate.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"cpbkviewstate.h" C:\S60\devices\*.mmp
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

- [API index — Phonebook UI Utilities API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_UI_Utilities_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phonebook+UI+Utilities+API%22)

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

<a id="src-10-s60-quick-reference-apis-082-phonebook-vcard-import-export-api-md"></a>
## Phonebook vCard Import Export API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/082_phonebook_vcard_import_export_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-082-phonebook-vcard-import-export-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 5th Ed

### Phonebook vCard Import Export API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Imports and exports vCards through the Phonebook engine. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Imports and exports vCards through the Phonebook engine.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 5th Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 5th Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `bcardeng.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <bcardeng.h>
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
findstr /s /n /i /c:"bcardeng.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"bcardeng.h" C:\S60\devices\*.mmp
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

- [API index — Phonebook vCard Import Export API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_vCard_Import_Export_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Phonebook+vCard+Import+Export+API%22)

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

<a id="src-10-s60-quick-reference-apis-083-pictograph-api-md"></a>
## Pictograph API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/083_pictograph_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-083-pictograph-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP2
- `deprecated`: none-listed

### Pictograph API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Renders platform pictographs in controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Renders platform pictographs in controls.

- First published: **S60 2nd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknpictographinterface.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpictographdrawerinterface.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknpictographinterface.h>
#include <aknpictographdrawerinterface.h>
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
findstr /s /n /i /c:"aknpictographinterface.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknpictographinterface.h" C:\S60\devices\*.mmp
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

- [API index — Pictograph API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Pictograph_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Pictograph+API%22)

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

<a id="src-10-s60-quick-reference-apis-084-platform-environment-api-md"></a>
## Platform Environment API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/084_platform_environment_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-084-platform-environment-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Platform Environment API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng PathInfo/path configuration để lấy đường dẫn chuẩn; không hard-code C:\Data, E:\Images hay tên thư mục localized.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Returns standard platform paths.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `PathInfo`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `pathinfo.h` | Data/value object or record definitions used by the API. |
| `pathconfiguration.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <pathinfo.h>
#include <pathconfiguration.hrh>
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

1. Lấy root/path từ PathInfo.
2. Kết hợp bằng TParse.
3. Kiểm tra drive/media bằng RFs.
4. Tạo thư mục nếu API/file policy cho phép.

#### 6. Build metadata

##### Import libraries

- `platformenv.lib`

```text
LIBRARY         platformenv.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"pathinfo.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"PathInfo" C:\S60\devices\*.mmp
```

##### Capabilities

None để lấy path; quyền file phụ thuộc nơi truy cập.

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
- Không giả định drive E tồn tại hoặc writable; không hiển thị raw path thay localized name.

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

- [API index — Platform Environment API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Platform_Environment_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Platform+Environment+API%22)

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

<a id="src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md"></a>
## Plug-in Bio Control API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/085_plug_in_bio_control_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Plug-in Bio Control API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Messaging BIO control plug-in interfaces. Dùng khi cần mở rộng platform qua ECom/provider/observer contract thay vì hard-code implementation vào app.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Messaging BIO control plug-in interfaces.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `msgbiocontrol.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msgbiocontrolobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mmsgbiocontrol.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msgbiouids.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `crichbio.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `mmsgbiocontrolextension.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <msgbiocontrol.h>
#include <msgbiocontrolobserver.h>
#include <mmsgbiocontrol.h>
#include <msgbiouids.h>
#include <crichbio.h>
#include <mmsgbiocontrolextension.h>
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
findstr /s /n /i /c:"msgbiocontrol.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"msgbiocontrol.h" C:\S60\devices\*.mmp
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

- [API index — Plug-in Bio Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Plug-in_Bio_Control_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Plug-in+Bio+Control+API%22)

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

<a id="src-10-s60-quick-reference-apis-086-popups-api-md"></a>
## Popups API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/086_popups_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-086-popups-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Popups API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Popup lists, fields, headings and setting pages. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Popup lists, fields, headings and setting pages.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknpopup.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupfader.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupfield.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupfieldlist.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupfieldtext.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupheadingpane.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopuplayout.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpopupsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknpopup.h>
#include <aknpopupfader.h>
#include <aknpopupfield.h>
#include <aknpopupfieldlist.h>
#include <aknpopupfieldtext.h>
#include <aknpopupheadingpane.h>
#include <aknpopuplayout.h>
#include <aknpopupsettingpage.h>
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
findstr /s /n /i /c:"aknpopup.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknpopup.h" C:\S60\devices\*.mmp
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

- [API index — Popups API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Popups_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Popups+API%22)

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

<a id="src-10-s60-quick-reference-apis-087-profiles-engine-active-profile-settings-api-md"></a>
## Profiles Engine Active Profile Settings API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/087_profiles_engine_active_profile_settings_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-087-profiles-engine-active-profile-settings-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Profiles Engine Active Profile Settings API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Read-only Central Repository keys for selected active-profile settings. Dùng khi cần truy cập database/model/UI dữ liệu platform để tương thích với contacts, calendar, messaging, landmarks hoặc media metadata.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Read-only Central Repository keys for selected active-profile settings.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `profileenginesdkcrkeys.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <profileenginesdkcrkeys.h>
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
findstr /s /n /i /c:"profileenginesdkcrkeys.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"profileenginesdkcrkeys.h" C:\S60\devices\*.mmp
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

- [API index — Profiles Engine Active Profile Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Profiles_Engine_Active_Profile_Settings_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Profiles+Engine+Active+Profile+Settings+API%22)

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

<a id="src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md"></a>
## Profiles Engine Wrapper API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/088_profiles_engine_wrapper_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP1
- `deprecated`: none-listed

### Profiles Engine Wrapper API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng wrapper để đọc/edit/activate profile có sẵn, không tự sửa repository keys không public.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Reads, edits and activates existing profiles and searches alert tones.

- First published: **S60 3rd Ed FP1**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `MProEngEngine`, `MProEngProfile`, `MProEngNotifyHandler`, `ProEngFactory`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `mproengactiveprofileobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mproengalerttoneseeker.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengalerttoneseekerobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mproengengine.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengnotifyhandler.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengprofile.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengprofileactivationobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mproengprofilename.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengprofilenamearray.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengprofilenamearrayobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mproengprofileobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `mproengtones.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `mproengtonesettings.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `proengfactory.h` | Factory entry point; normally creates an interface or implementation selected at runtime. |
| `profile.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <mproengactiveprofileobserver.h>
#include <mproengalerttoneseeker.h>
#include <mproengalerttoneseekerobserver.h>
#include <mproengengine.h>
#include <mproengnotifyhandler.h>
#include <mproengprofile.h>
#include <mproengprofileactivationobserver.h>
#include <mproengprofilename.h>
#include <mproengprofilenamearray.h>
#include <mproengprofilenamearrayobserver.h>
#include <mproengprofileobserver.h>
#include <mproengtones.h>
// + 3 header khác; chỉ include header chứa symbol thực sự dùng.
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

1. Create engine bằng factory.
2. Lấy active/profile names.
3. Open profile object.
4. Edit supported settings và commit.
5. Observe activation/profile changes.

#### 6. Build metadata

##### Import libraries

- Xác minh local SDK

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"mproengactiveprofileobserver.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"MProEngEngine" C:\S60\devices\*.mmp
```

##### Capabilities

Operation-specific.

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
- Không giữ interface sau khi engine chết; không tự tạo profile nếu API chỉ hỗ trợ existing profiles.

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

- [API index — Profiles Engine Wrapper API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Profiles_Engine_Wrapper_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Profiles+Engine+Wrapper+API%22)

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

<a id="src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md"></a>
## Predictive Input PTI Engine API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/089_predictive_input_pti_engine_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Predictive Input PTI Engine API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Predictive-text engine, languages, mappings and user dictionaries. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Predictive-text engine, languages, mappings and user dictionaries.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `ptidefs.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `ptiengine.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |
| `ptilanguage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `ptikeymappings.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `ptiuserdictionary.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `ptiobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |
| `pticompositiondataif.h` | Data/value object or record definitions used by the API. |
| `ptiuserdicentry.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `ptilanguagedatabase.h` | Data/value object or record definitions used by the API. |
| `ptiuids.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <ptidefs.h>
#include <ptiengine.h>
#include <ptilanguage.h>
#include <ptikeymappings.h>
#include <ptiuserdictionary.h>
#include <ptiobserver.h>
#include <pticompositiondataif.h>
#include <ptiuserdicentry.h>
#include <ptilanguagedatabase.h>
#include <ptiuids.hrh>
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
findstr /s /n /i /c:"ptidefs.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"ptidefs.h" C:\S60\devices\*.mmp
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

- [API index — Predictive Input PTI Engine API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Predictive_Input_PTI_Engine_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Predictive+Input+PTI+Engine+API%22)

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

<a id="src-10-s60-quick-reference-apis-090-queries-api-md"></a>
## Queries API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/090_queries_api.md`
- Consolidated reference: `23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Queries API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng Avkon query resources/classes cho confirmation, text, number, list, date/time; không dựng dialog thủ công.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Avkon confirmation, list, message and typed-value queries.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CAknQueryDialog`, `CAknListQueryDialog`, `CAknMessageQueryDialog`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akndialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknlistquerycontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknlistquerydialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknmessagequerycontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknmessagequerydialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknmultilinequerycontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknquerycontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknquerydata.h` | Data/value object or record definitions used by the API. |
| `aknquerydialog.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknqueryvalue.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknqueryvaluedate.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknqueryvalueduration.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknqueryvaluenumber.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknqueryvaluephone.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknqueryvaluetext.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknqueryvaluetime.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akndialog.h>
#include <aknlistquerycontrol.h>
#include <aknlistquerydialog.h>
#include <aknmessagequerycontrol.h>
#include <aknmessagequerydialog.h>
#include <aknmultilinequerycontrol.h>
#include <aknquerycontrol.h>
#include <aknquerydata.h>
#include <aknquerydialog.h>
#include <aknqueryvalue.h>
#include <aknqueryvaluedate.h>
#include <aknqueryvalueduration.h>
// + 4 header khác; chỉ include header chứa symbol thực sự dùng.
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

1. Khai báo resource đúng query type.
2. Bind buffer/value.
3. ExecuteLD/RunLD.
4. Xử lý cancel riêng với empty input.
5. Validate length/range.

#### 6. Build metadata

##### Import libraries

- `avkon.lib`

```text
LIBRARY         avkon.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"akndialog.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CAknQueryDialog" C:\S60\devices\*.mmp
```

##### Capabilities

None.

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
- Không dùng resource type không khớp C++ class; dễ panic/RCOMP lỗi.

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

- [API index — Queries API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Queries_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Queries+API%22)

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
