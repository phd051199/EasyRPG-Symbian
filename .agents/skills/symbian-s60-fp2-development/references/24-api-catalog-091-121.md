# Complete API usage guides 091–121

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-10-s60-quick-reference-apis-091-rop-custom-commands-api-md"></a>
## ROP Custom Commands API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/091_rop_custom_commands_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-091-rop-custom-commands-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: S60 3rd Ed

### ROP Custom Commands API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Constants for video-player custom commands with the ROP plug-in. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Constants for video-player custom commands with the ROP plug-in.

- First published: **S60 2nd Ed**
- Deprecated marker: **S60 3rd Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 3rd Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `mmfropcustomcommandconstants.h` | Shared constants, enums, UIDs, error codes or common value types. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <mmfropcustomcommandconstants.h>
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
findstr /s /n /i /c:"mmfropcustomcommandconstants.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"mmfropcustomcommandconstants.h" C:\S60\devices\*.mmp
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

- [API index — ROP Custom Commands API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/ROP_Custom_Commands_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22ROP+Custom+Commands+API%22)

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

<a id="src-10-s60-quick-reference-apis-092-scalable-icons-api-md"></a>
## Scalable Icons API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/092_scalable_icons_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-092-scalable-icons-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed FP3
- `deprecated`: none-listed

### Scalable Icons API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng AknIconUtils/MIF/SVG để load và scale icon skin-aware; không tự rasterize mọi kích thước.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Loads and scales SVG/MIF and bitmap icons.

- First published: **S60 2nd Ed FP3**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `AknIconUtils`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akniconutils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akniconsrvclient.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akniconobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akniconutils.h>
#include <akniconsrvclient.h>
#include <akniconobserver.h>
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

1. CreateIconL bitmap+mask.
2. SetSize với aspect-ratio mode.
3. Sở hữu/xoá cả bitmap và mask.
4. Re-scale khi layout thay đổi.

#### 6. Build metadata

##### Import libraries

- `avkon.lib`

```text
LIBRARY         avkon.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"akniconutils.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"AknIconUtils" C:\S60\devices\*.mmp
```

##### Capabilities

None.

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
- Không scale trong mỗi Draw(); không bỏ mask ownership.

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

- [API index — Scalable Icons API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Scalable_Icons_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Scalable+Icons+API%22)

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

<a id="src-10-s60-quick-reference-apis-093-screen-clearer-api-md"></a>
## Screen Clearer API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/093_screen_clearer_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-093-screen-clearer-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Screen Clearer API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Clears or blanks screen areas. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Clears or blanks screen areas.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknclearer.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikspace.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknclearer.h>
#include <eikspace.h>
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
findstr /s /n /i /c:"aknclearer.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknclearer.h" C:\S60\devices\*.mmp
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

- [API index — Screen Clearer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Screen_Clearer_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Screen+Clearer+API%22)

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

<a id="src-10-s60-quick-reference-apis-094-screen-mode-api-md"></a>
## Screen Mode API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/094_screen_mode_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-094-screen-mode-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP1
- `deprecated`: none-listed

### Screen Mode API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Queries product screen-mode data. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Queries product screen-mode data.

- First published: **S60 3rd Ed FP1**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknscreenmode.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknscreenmode.h>
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
findstr /s /n /i /c:"aknscreenmode.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknscreenmode.h" C:\S60\devices\*.mmp
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

- [API index — Screen Mode API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Screen_Mode_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Screen+Mode+API%22)

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

<a id="src-10-s60-quick-reference-apis-095-screen-saver-api-md"></a>
## Screen Saver API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/095_screen_saver_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-095-screen-saver-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Screen Saver API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Screen-saver plug-in interface. Dùng khi cần mở rộng platform qua ECom/provider/observer contract thay vì hard-code implementation vào app.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Screen-saver plug-in interface.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `screensaverplugin.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `screensaverpluginintdef.h` | Plug-in/adapter contract; check ECom registration, implementation UID and resource metadata. |
| `screensaverpluginintdef.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <screensaverplugin.h>
#include <screensaverpluginintdef.h>
#include <screensaverpluginintdef.hrh>
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
findstr /s /n /i /c:"screensaverplugin.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"screensaverplugin.h" C:\S60\devices\*.mmp
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

- [API index — Screen Saver API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Screen_Saver_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Screen+Saver+API%22)

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

<a id="src-10-s60-quick-reference-apis-096-scroller-api-md"></a>
## Scroller API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/096_scroller_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-096-scroller-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Scroller API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Scroll bars, frames and observers. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Scroll bars, frames and observers.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknscbut.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknscrlb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiksbfrm.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eiksbobs.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikscbut.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikscrlb.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknscbut.h>
#include <aknscrlb.h>
#include <eiksbfrm.h>
#include <eiksbobs.h>
#include <eikscbut.h>
#include <eikscrlb.h>
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
findstr /s /n /i /c:"aknscbut.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknscbut.h" C:\S60\devices\*.mmp
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

- [API index — Scroller API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Scroller_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Scroller+API%22)

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

<a id="src-10-s60-quick-reference-apis-097-searchfield-api-md"></a>
## Searchfield API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/097_searchfield_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-097-searchfield-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Searchfield API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Search-field control for filtering lists. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Search-field control for filtering lists.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknsfld.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknsfld.h>
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
findstr /s /n /i /c:"aknsfld.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknsfld.h" C:\S60\devices\*.mmp
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

- [API index — Searchfield API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Searchfield_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Searchfield+API%22)

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

<a id="src-10-s60-quick-reference-apis-098-send-ui-api-md"></a>
## Send UI API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/098_send_ui_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Send UI API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng để tái sử dụng “Send via” và editor/service có sẵn thay vì tự viết SMTP/MMS/Bluetooth UI. App chỉ chuẩn bị CMessageData và để platform chọn service phù hợp.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Builds Send via menus and launches supported message editors.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CSendUi`, `CMessageData`, `CMessageAddress`, `TSendingCapabilities`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `sendui.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `tsendingcapabilities.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `cmessagedata.h` | Data/value object or record definitions used by the API. |
| `cmessageaddress.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senduimtmuids.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `senduiconsts.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `csendingserviceinfo.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <sendui.h>
#include <tsendingcapabilities.h>
#include <cmessagedata.h>
#include <cmessageaddress.h>
#include <senduimtmuids.h>
#include <senduiconsts.h>
#include <csendingserviceinfo.h>
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

1. Tạo CSendUi.
2. Tạo CMessageData, body/subject/recipient/attachment.
3. Lọc service theo TSendingCapabilities nếu cần.
4. AddSendMenuItemL hoặc ShowQueryAndSendL/CreateAndSendMessageL.
5. Giữ attachment hợp lệ cho đến khi service/editor nhận xong.

#### 6. Build metadata

##### Import libraries

- `sendui.lib`

```text
LIBRARY         sendui.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"sendui.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CSendUi" C:\S60\devices\*.mmp
```

##### Capabilities

Phụ thuộc dữ liệu/attachment được đọc; kiểm tra member và đường dẫn cụ thể.

Capability là thuộc tính của **operation/member/server policy**, không phải của cả header.
Chỉ thêm capability sau khi xác định method thực sự gọi.

#### 7. Ví dụ/skeleton

```cpp
_LIT(KSubject, "Subject");
_LIT(KBody, "Body");
CSendUi* sendUi = CSendUi::NewLC();
CMessageData* data = CMessageData::NewLC();
data->SetSubjectL(&KSubject);
data->SetBodyTextL(&KBody);
sendUi->ShowQueryAndSendL(data);
CleanupStack::PopAndDestroy(2, sendUi);
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
- Không tự dựng menu transport cứng; không giả định mọi máy có email/MMS account; không xoá file attachment quá sớm.

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

- [API index — Send UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Send_UI_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Send+UI+API%22)

##### API-specific subpages

- [Send_UI_API2.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Send_UI_API2.html)
- [Send_UI_API4.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Send_UI_API4.html)
- [Send_UI_APIClassesIndexPage.html](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Send_UI_APIClassesIndexPage.html)

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

<a id="src-10-s60-quick-reference-apis-099-server-application-api-md"></a>
## Server Application API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/099_server_application_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-099-server-application-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Server Application API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Client/server application integration for document-style services. Dùng khi cần mở rộng platform qua ECom/provider/observer contract thay vì hard-code implementation vào app.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Client/server application integration for document-style services.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknserverapp.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknserverapp.h>
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
findstr /s /n /i /c:"aknserverapp.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknserverapp.h" C:\S60\devices\*.mmp
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

- [API index — Server Application API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Server_Application_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Server+Application+API%22)

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

<a id="src-10-s60-quick-reference-apis-100-setting-pages-api-md"></a>
## Setting Pages API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/100_setting_pages_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-100-setting-pages-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Setting Pages API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng setting pages/CAknSettingItemList cho UX settings chuẩn.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Setting pages and setting-item lists.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CAknSettingItemList`, `CAknSettingItem`, `CAknTextSettingPage`, `CAknRadioButtonSettingPage`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akncheckboxsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknpasswordsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akntextsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknslidersettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknmfnesettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsettingitemlist.h` | Data/value object or record definitions used by the API. |
| `aknsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknvolumesettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknradiobuttonsettingpage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akndescarraydecorator.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akncheckboxsettingpage.h>
#include <aknpasswordsettingpage.h>
#include <akntextsettingpage.h>
#include <aknslidersettingpage.h>
#include <aknmfnesettingpage.h>
#include <aknsettingitemlist.h>
#include <aknsettingpage.h>
#include <aknvolumesettingpage.h>
#include <aknradiobuttonsettingpage.h>
#include <akndescarraydecorator.h>
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

1. Khai báo setting item resources.
2. CreateSettingItemL theo identifier.
3. LoadSettingsL.
4. EditItemL.
5. StoreSettingsL rồi persist Central Repository/file.

#### 6. Build metadata

##### Import libraries

- `avkon.lib`

```text
LIBRARY         avkon.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"akncheckboxsettingpage.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CAknSettingItemList" C:\S60\devices\*.mmp
```

##### Capabilities

None cho UI; persistence tùy backend.

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
- Không dùng UI state làm source of truth lâu dài; persist riêng.

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

- [API index — Setting Pages API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Setting_Pages_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Setting+Pages+API%22)

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

<a id="src-10-s60-quick-reference-apis-101-skins-api-md"></a>
## Skins API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/101_skins_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-101-skins-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Skins API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng Akns draw utilities/control contexts để app hợp skin, thay vì hard-code màu/background.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Skin instances, item IDs, drawing utilities and background contexts.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `MAknsSkinInstance`, `AknsUtils`, `AknsDrawUtils`, `CAknsBasicBackgroundControlContext`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknsskininstance.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsitemdata.h` | Data/value object or record definitions used by the API. |
| `aknsutils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsdrawutils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsitemid.h` | Data/value object or record definitions used by the API. |
| `aknsconstants.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `aknsconstants.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |
| `aknsbasicbackgroundcontrolcontext.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknslistboxbackgroundcontrolcontext.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknslayeredbackgroundcontrolcontext.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknsframebackgroundcontrolcontext.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknscontrolcontext.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |
| `aknsdatacontext.h` | Data/value object or record definitions used by the API. |
| `aknsrleffect.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsrleffectcontext.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsrlparameter.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknswallpaperutils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknsskininstance.h>
#include <aknsitemdata.h>
#include <aknsutils.h>
#include <aknsdrawutils.h>
#include <aknsitemid.h>
#include <aknsconstants.h>
#include <aknsconstants.hrh>
#include <aknsbasicbackgroundcontrolcontext.h>
#include <aknslistboxbackgroundcontrolcontext.h>
#include <aknslayeredbackgroundcontrolcontext.h>
#include <aknsframebackgroundcontrolcontext.h>
#include <aknscontrolcontext.h>
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

1. Lấy skin instance.
2. Tạo control context phù hợp.
3. Draw background trong Draw().
4. Update context khi SizeChanged.
5. Fallback system colors nếu skin item thiếu.

#### 6. Build metadata

##### Import libraries

- aknssrv.lib / avkon.lib; xác minh class cụ thể

```text
; Library: local SDK/class-page check required
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"aknsskininstance.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"MAknsSkinInstance" C:\S60\devices\*.mmp
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
- Không cache màu skin vĩnh viễn; skin có thể đổi runtime.

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

- [API index — Skins API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Skins_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Skins+API%22)

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

<a id="src-10-s60-quick-reference-apis-102-slider-api-md"></a>
## Slider API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/102_slider_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-102-slider-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Slider API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Slider control. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Slider control.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknslider.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknslider.h>
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
findstr /s /n /i /c:"aknslider.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknslider.h" C:\S60\devices\*.mmp
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

- [API index — Slider API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Slider_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Slider+API%22)

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

<a id="src-10-s60-quick-reference-apis-103-sounds-api-md"></a>
## Sounds API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/103_sounds_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-103-sounds-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Sounds API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Key and UI sound utilities. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Key and UI sound utilities.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknsoundsystem.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `aknsoundinfo.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknsoundsystem.h>
#include <aknsoundinfo.h>
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
findstr /s /n /i /c:"aknsoundsystem.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknsoundsystem.h" C:\S60\devices\*.mmp
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

- [API index — Sounds API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Sounds_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Sounds+API%22)

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

<a id="src-10-s60-quick-reference-apis-104-speed-dial-utility-control-api-md"></a>
## Speed Dial Utility Control API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/104_speed_dial_utility_control_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-104-speed-dial-utility-control-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: S60 3rd Ed FP2

### Speed Dial Utility Control API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy access to speed-dial configuration. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy access to speed-dial configuration.

- First published: **S60 1st Ed**
- Deprecated marker: **S60 3rd Ed FP2**
- Nhận xét: API được đánh dấu deprecated từ **S60 3rd Ed FP2**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `spdiacontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <spdiacontrol.h>
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
findstr /s /n /i /c:"spdiacontrol.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"spdiacontrol.h" C:\S60\devices\*.mmp
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

- [API index — Speed Dial Utility Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Speed_Dial_Utility_Control_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Speed+Dial+Utility+Control+API%22)

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

<a id="src-10-s60-quick-reference-apis-105-startup-list-management-api-md"></a>
## Startup List Management API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/105_startup_list_management_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-105-startup-list-management-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Startup List Management API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Installer-time declarations for trusted startup-list changes. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Installer-time declarations for trusted startup-list changes.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `startupitem.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <startupitem.hrh>
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
findstr /s /n /i /c:"startupitem.hrh" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"startupitem.hrh" C:\S60\devices\*.mmp
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

- [API index — Startup List Management API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Startup_List_Management_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Startup+List+Management+API%22)

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

<a id="src-10-s60-quick-reference-apis-106-status-pane-api-md"></a>
## Status Pane API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/106_status_pane_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-106-status-pane-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Status Pane API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Status pane layouts and controls. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Status pane layouts and controls.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `eikspane.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `eikspmod.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <eikspane.h>
#include <eikspmod.h>
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
findstr /s /n /i /c:"eikspane.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"eikspane.h" C:\S60\devices\*.mmp
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

- [API index — Status Pane API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Status_Pane_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Status+Pane+API%22)

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

<a id="src-10-s60-quick-reference-apis-107-string-loader-api-md"></a>
## String Loader API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/107_string_loader_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-107-string-loader-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### String Loader API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng StringLoader để nạp chuỗi localized và thay placeholder; không hard-code text trong C++.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Loads and substitutes parameters in localized resource strings.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `StringLoader`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `stringloader.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <stringloader.h>
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

1. Đặt string trong RSS/loc.
2. LoadL/LoadLC đúng overload.
3. Truyền tham số khớp placeholder.
4. Cleanup HBufC trả về.

#### 6. Build metadata

##### Import libraries

- `commonengine.lib`

```text
LIBRARY         commonengine.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"stringloader.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"StringLoader" C:\S60\devices\*.mmp
```

##### Capabilities

None.

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
- Không Format() chuỗi localized với thứ tự placeholder tự đoán.

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

- [API index — String Loader API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/String_Loader_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22String+Loader+API%22)

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

<a id="src-10-s60-quick-reference-apis-108-system-utils-api-md"></a>
## System Utils API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/108_system_utils_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-108-system-utils-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### System Utils API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng SysUtil cho version/disk critical checks thay vì tự đo ngưỡng hoặc parse file hệ thống.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Reads software/language versions and checks disk space.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `SysUtil`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `sysutil.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <sysutil.h>
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

1. Kiểm tra disk space trước ghi lớn.
2. Dùng RFs session hợp lệ.
3. Xử lý threshold và removable media.
4. Báo lỗi theo platform UI.

#### 6. Build metadata

##### Import libraries

- `sysutil.lib`

```text
LIBRARY         sysutil.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"sysutil.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"SysUtil" C:\S60\devices\*.mmp
```

##### Capabilities

Tùy method/path.

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
- Không chỉ kiểm tra free bytes một lần rồi ghi không giới hạn.

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

- [API index — System Utils API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/System_Utils_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22System+Utils+API%22)

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

<a id="src-10-s60-quick-reference-apis-109-tabs-api-md"></a>
## Tabs API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/109_tabs_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-109-tabs-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Tabs API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Tab group and observer interface. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Tab group and observer interface.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akntabgrp.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `akntabobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akntabgrp.h>
#include <akntabobserver.h>
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
findstr /s /n /i /c:"akntabgrp.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akntabgrp.h" C:\S60\devices\*.mmp
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

- [API index — Tabs API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Tabs_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Tabs+API%22)

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

<a id="src-10-s60-quick-reference-apis-110-text-resolver-api-md"></a>
## Text Resolver API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/110_text_resolver_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-110-text-resolver-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: none-listed

### Text Resolver API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng CTextResolver để biến system error thành text localized; vẫn log numeric error để debug.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Maps error codes to localized text.

- First published: **S60 2nd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CTextResolver`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `textresolver.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `textresolver.hrh` | Resource/command/enum constants for `.rss`, `.hrh` and C++ integration. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <textresolver.h>
#include <textresolver.hrh>
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

1. Tạo resolver với CEikonEnv.
2. ResolveErrorString().
3. Hiển thị note phù hợp.
4. Log error code gốc.

#### 6. Build metadata

##### Import libraries

- `commonengine.lib`

```text
LIBRARY         commonengine.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"textresolver.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CTextResolver" C:\S60\devices\*.mmp
```

##### Capabilities

None.

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
- Không coi text resolver là logic recovery.

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

- [API index — Text Resolver API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Text_Resolver_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Text+Resolver+API%22)

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

<a id="src-10-s60-quick-reference-apis-111-title-pane-api-md"></a>
## Title Pane API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/111_title_pane_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-111-title-pane-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Title Pane API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Controls the status-pane title. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Controls the status-pane title.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akntitle.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akntitle.h>
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
findstr /s /n /i /c:"akntitle.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akntitle.h" C:\S60\devices\*.mmp
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

- [API index — Title Pane API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Title_Pane_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Title+Pane+API%22)

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

<a id="src-10-s60-quick-reference-apis-112-touch-pane-observer-api-md"></a>
## Touch Pane Observer API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/112_touch_pane_observer_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-112-touch-pane-observer-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Touch Pane Observer API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Observes touch-pane geometry changes; hardware usefulness is device-dependent. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Observes touch-pane geometry changes; hardware usefulness is device-dependent.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `akntouchpaneobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <akntouchpaneobserver.h>
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
findstr /s /n /i /c:"akntouchpaneobserver.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"akntouchpaneobserver.h" C:\S60\devices\*.mmp
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

- [API index — Touch Pane Observer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Touch_Pane_Observer_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Touch+Pane+Observer+API%22)

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

<a id="src-10-s60-quick-reference-apis-113-version-info-api-md"></a>
## Version Info API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/113_version_info_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-113-version-info-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed FP2
- `deprecated`: none-listed

### Version Info API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng để đọc version platform/system khi thật sự cần; feature detection vẫn ưu tiên hơn.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Queries platform and system version information.

- First published: **S60 3rd Ed FP2**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CVersionInfo`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `versioninfo.h` | Data/value object or record definitions used by the API. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <versioninfo.h>
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

1. Chọn version type.
2. Gọi GetVersion().
3. Xử lý KErrNotSupported/KErrNotFound.
4. Không dùng version để thay thế capability/feature probe.

#### 6. Build metadata

##### Import libraries

- `platformenv.lib`

```text
LIBRARY         platformenv.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"versioninfo.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CVersionInfo" C:\S60\devices\*.mmp
```

##### Capabilities

None cho thông tin version thông thường.

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
- Không parse chuỗi firmware bằng offset cố định.

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

- [API index — Version Info API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Version_Info_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Version+Info+API%22)

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

<a id="src-10-s60-quick-reference-apis-114-vibra-api-md"></a>
## Vibra API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/114_vibra_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-114-vibra-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Vibra API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng HWRM Vibra; tôn trọng profile và device support.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Controls vibration through HWRM.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: `CHWRMVibra`

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `hwrmvibra.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <hwrmvibra.h>
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

1. NewL(observer nếu cần).
2. Check status/availability.
3. StartVibraL(duration,intensity).
4. StopVibraL khi lifecycle thay đổi.
5. Fallback im lặng nếu bị policy chặn.

#### 6. Build metadata

##### Import libraries

- `hwrmvibraclient.lib`

```text
LIBRARY         hwrmvibraclient.lib
```

Nếu library chưa được xác minh, không đoán theo tên header. Chạy:

```bat
findstr /s /n /i /c:"hwrmvibra.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"CHWRMVibra" C:\S60\devices\*.mmp
```

##### Capabilities

Device/operation policy.

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
- Không loop vibration bằng timer dày; không giả định user cho phép.

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

- [API index — Vibra API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Vibra_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Vibra+API%22)

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

<a id="src-10-s60-quick-reference-apis-115-vibra-control-api-md"></a>
## Vibra Control API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/115_vibra_control_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-115-vibra-control-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 2nd Ed
- `deprecated`: S60 3rd Ed

### Vibra Control API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Older vibration-control API. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Older vibration-control API.

- First published: **S60 2nd Ed**
- Deprecated marker: **S60 3rd Ed**
- Nhận xét: API được đánh dấu deprecated từ **S60 3rd Ed**. Với code mới, tìm replacement trong Changes/Usage trước khi dùng.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `vibractrl.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <vibractrl.h>
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
findstr /s /n /i /c:"vibractrl.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"vibractrl.h" C:\S60\devices\*.mmp
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

- [API index — Vibra Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Vibra_Control_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Vibra+Control+API%22)

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

<a id="src-10-s60-quick-reference-apis-116-volume-control-api-md"></a>
## Volume Control API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/116_volume_control_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-116-volume-control-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Volume Control API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Volume-control UI component. Dùng khi cần hành vi/giao diện chuẩn Avkon, keyboard navigation, layout, skin và resource integration. Ưu tiên control/dialog/list có sẵn trước khi viết CCoeControl tùy biến.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Volume-control UI component.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknvolumecontrol.h` | UI facade/control/dialog declaration; must run on the UI thread with a CONE environment. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknvolumecontrol.h>
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
findstr /s /n /i /c:"aknvolumecontrol.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknvolumecontrol.h" C:\S60\devices\*.mmp
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

- [API index — Volume Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Volume_Control_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Volume+Control+API%22)

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

<a id="src-10-s60-quick-reference-apis-117-web-service-connection-api-md"></a>
## Web Service Connection API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/117_web_service_connection_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Web Service Connection API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Creates service connections and sends SOAP/web-service messages. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Creates service connections and sends SOAP/web-service messages.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `msenserviceconsumer.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `senserviceconnection.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sendateutils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sensoapenvelope.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sensoapfault.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sensoapmessage.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senwssecurityheader.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sensoapconstants.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `msenauthenticationprovider.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenproperty.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenproperties.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `senxmlproperties.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sentransportproperties.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senhttptransportproperties.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <msenserviceconsumer.h>
#include <senserviceconnection.h>
#include <sendateutils.h>
#include <sensoapenvelope.h>
#include <sensoapfault.h>
#include <sensoapmessage.h>
#include <senwssecurityheader.h>
#include <sensoapconstants.h>
#include <msenauthenticationprovider.h>
#include <msenproperty.h>
#include <msenproperties.h>
#include <senxmlproperties.h>
// + 2 header khác; chỉ include header chứa symbol thực sự dùng.
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
findstr /s /n /i /c:"msenserviceconsumer.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"msenserviceconsumer.h" C:\S60\devices\*.mmp
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

- [API index — Web Service Connection API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Web_Service_Connection_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Web+Service+Connection+API%22)

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

<a id="src-10-s60-quick-reference-apis-118-web-service-description-api-md"></a>
## Web Service Description API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/118_web_service_description_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Web Service Description API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Models service descriptions, policies, credentials and identity providers. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Models service descriptions, policies, credentials and identity providers.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `msenconsumerpolicy.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenidentityprovideridarray.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenproviderpolicy.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenservicedescription.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `sencredential.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senidentityprovider.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senidentityprovideridarray8.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senservicepattern.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senxmlservicedescription.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senfacet.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `msenservicepolicy.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `sencredential2.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <msenconsumerpolicy.h>
#include <msenidentityprovideridarray.h>
#include <msenproviderpolicy.h>
#include <msenservicedescription.h>
#include <sencredential.h>
#include <senidentityprovider.h>
#include <senidentityprovideridarray8.h>
#include <senservicepattern.h>
#include <senxmlservicedescription.h>
#include <senfacet.h>
#include <msenservicepolicy.h>
#include <sencredential2.h>
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
findstr /s /n /i /c:"msenconsumerpolicy.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"msenconsumerpolicy.h" C:\S60\devices\*.mmp
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

- [API index — Web Service Description API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Web_Service_Description_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Web+Service+Description+API%22)

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

<a id="src-10-s60-quick-reference-apis-119-web-service-manager-api-md"></a>
## Web Service Manager API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/119_web_service_manager_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-119-web-service-manager-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### Web Service Manager API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Stores and retrieves web-service and identity information. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Stores and retrieves web-service and identity information.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `senservicemanager.h` | Manager/session/engine entry point; inspect Open/NewL/Close lifecycle and server connection semantics. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <senservicemanager.h>
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
findstr /s /n /i /c:"senservicemanager.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"senservicemanager.h" C:\S60\devices\*.mmp
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

- [API index — Web Service Manager API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Web_Service_Manager_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Web+Service+Manager+API%22)

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

<a id="src-10-s60-quick-reference-apis-120-window-server-event-observer-api-md"></a>
## Window Server Event Observer API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/120_window_server_event_observer_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-120-window-server-event-observer-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 1st Ed
- `deprecated`: none-listed

### Window Server Event Observer API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Receives selected Window Server events outside the normal control stack. Dùng helper/constant/observer/platform abstraction có sẵn để tránh duplicate logic, hard-code path/version/error hoặc device-specific behavior.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Receives selected Window Server events outside the normal control stack.

- First published: **S60 1st Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `aknwseventobserver.h` | Observer/callback interface; implement it and keep the observer alive through asynchronous notifications. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <aknwseventobserver.h>
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
findstr /s /n /i /c:"aknwseventobserver.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"aknwseventobserver.h" C:\S60\devices\*.mmp
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

- [API index — Window Server Event Observer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Window_Server_Event_Observer_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22Window+Server+Event+Observer+API%22)

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

<a id="src-10-s60-quick-reference-apis-121-xml-extensions-api-md"></a>
## XML Extensions API

- Original corpus path: `10_S60_QUICK_REFERENCE_APIS/121_xml_extensions_api.md`
- Consolidated reference: `24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md`
- `kind`: complete_api_usage_guide
- `target`: S60 3rd Edition Feature Pack 2 / Symbian OS 9.3
- `published`: S60 3rd Ed
- `deprecated`: none-listed

### XML Extensions API

#### 1. Kết luận nhanh: khi nào nên tái sử dụng API này

Dùng API này khi bài toán khớp mục đích: Legacy web-services XML element, fragment and reader helpers. Dùng để gọi service/platform server hoặc utility chuẩn thay vì truy cập private repository/file/protocol nội bộ.

**Đừng tự xây lại** phần platform đã cung cấp: discovery, lifecycle, UI convention,
server IPC, persistence, codec, filtering hoặc error mapping tương ứng. Chỉ viết lớp
adapter/domain riêng ở phía app.

#### 2. Mục đích đã đối chiếu

Legacy web-services XML element, fragment and reader helpers.

- First published: **S60 3rd Ed**
- Deprecated marker: **none-listed**
- Nhận xét: Không bị Quick Reference đánh dấu deprecated trong release này.
- Entry point/class đáng kiểm tra trước: Mở **Classes and Definitions** trong API index và chọn entry point từ các header bên dưới.

#### 3. Public headers và vai trò

| Header | Vai trò thực hành |
|---|---|
| `msencontenthandlerclient.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenelement.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `msenfragment.h` | Likely mixin/interface header; implement callbacks and follow ownership stated by the concrete API. |
| `senbaseattribute.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senbaseelement.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senbasefragment.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sendomfragment.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senelement.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senfragment.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `sennamespace.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senxmlconstants.h` | Shared constants, enums, UIDs, error codes or common value types. |
| `senxmlreader.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senxmlutils.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |
| `senxmlelement.h` | Public declaration header. Inspect classes, exported functions, ownership comments and availability guards. |

Không include toàn bộ chỉ vì chúng nằm cùng API group. Include tối thiểu giúp phát hiện
nhầm edition và dependency ẩn sớm hơn.

```cpp
#include <msencontenthandlerclient.h>
#include <msenelement.h>
#include <msenfragment.h>
#include <senbaseattribute.h>
#include <senbaseelement.h>
#include <senbasefragment.h>
#include <sendomfragment.h>
#include <senelement.h>
#include <senfragment.h>
#include <sennamespace.h>
#include <senxmlconstants.h>
#include <senxmlreader.h>
// + 2 header khác; chỉ include header chứa symbol thực sự dùng.
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
findstr /s /n /i /c:"msencontenthandlerclient.h" "%EPOCROOT%epoc32\include\*.h"
findstr /s /n /i /c:"msencontenthandlerclient.h" C:\S60\devices\*.mmp
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

- [API index — XML Extensions API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/XML_Extensions_APIIndexPage.html)
- [S60 Quick Reference — API/header/version inventory](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API Reference index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public header/file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
- [Class hierarchy](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/hierarchy.html)
- [Data structures/classes](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/annotated.html)
- [Member index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/functions.html)
- [Deprecated API list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/deprecated.html)
- [S60 API usage rules](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/api_usage_rules.html)
- [Guide to the S60 Developer’s Library](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-838A90DB-08C4-4A79-A234-43891C47F268.html)
- [Search locator for this API](https://www.google.com/search?q=site%3Adocs.huihoo.com%2Fsymbian%2Fs60-3rd-edition-cpp-developers-library-v1.1+%22XML+Extensions+API%22)

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
