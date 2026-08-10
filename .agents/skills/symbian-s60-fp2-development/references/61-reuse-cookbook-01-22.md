# Reuse cookbook recipes 01–22

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-21-reuse-cookbook-001-kh-i-t-o-app-avkon-ng-chu-n-md"></a>
## Khởi tạo app Avkon đúng chuẩn

- Original corpus path: `21_REUSE_COOKBOOK/001_kh_i_t_o_app_avkon_ng_chu_n.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-001-kh-i-t-o-app-avkon-ng-chu-n-md`
- `kind`: reuse_recipe

### Khởi tạo app Avkon đúng chuẩn

#### Bài toán

Tạo native GUI app có menu, softkey và lifecycle.

#### API/component nên tái sử dụng

- [Application Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-007-application-framework-api-md)
- [Options Menu API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-075-options-menu-api-md)
- [Status Pane API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-106-status-pane-api-md)

#### Quyết định kiến trúc

Dùng CAknApplication/CAknDocument/CAknAppUi; resource registration và control stack.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: euser.lib, apparc.lib, cone.lib, eikcore.lib, avkon.lib
- Capabilities: None cho shell UI

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Đừng tự tạo event loop/window group nếu Avkon app framework đủ dùng.

#### Guide/class liên quan trong corpus

- [AVKON APP LIFECYCLE](13-deep-guides.md#src-14-deep-guides-avkon-app-lifecycle-md)
- [022 caknapplication](41-class-guides-001-025.md#src-12-class-guides-022-caknapplication-md)
- [024 caknappui](41-class-guides-001-025.md#src-12-class-guides-024-caknappui-md)

#### Link chính thức

- [Application Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Application_Framework_APIIndexPage.html)
- [Options Menu API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Options_Menu_APIIndexPage.html)
- [Status Pane API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Status_Pane_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-002-nhi-u-m-n-h-nh-v-navigation-md"></a>
## Nhiều màn hình và navigation

- Original corpus path: `21_REUSE_COOKBOOK/002_nhi_u_m_n_h_nh_v_navigation.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-002-nhi-u-m-n-h-nh-v-navigation-md`
- `kind`: reuse_recipe

### Nhiều màn hình và navigation

#### Bài toán

Tách app phức tạp thành nhiều screen có activate/deactivate.

#### API/component nên tái sử dụng

- [Application Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-007-application-framework-api-md)
- [Navigation Pane API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-068-navigation-pane-api-md)
- [Tabs API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-109-tabs-api-md)

#### Quyết định kiến trúc

Dùng CAknViewAppUi + CAknView, mỗi view quản lý container ngắn hạn.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib, eikcore.lib
- Capabilities: Theo service mỗi view gọi

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không giữ container sau DoDeactivate; truyền state qua model/service.

#### Guide/class liên quan trong corpus

- [025 caknview](41-class-guides-001-025.md#src-12-class-guides-025-caknview-md)
- [026 caknviewappui](42-class-guides-026-050.md#src-12-class-guides-026-caknviewappui-md)

#### Link chính thức

- [Application Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Application_Framework_APIIndexPage.html)
- [Navigation Pane API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Navigation_Pane_APIIndexPage.html)
- [Tabs API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Tabs_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-003-menu-ng-theo-tr-ng-th-i-md"></a>
## Menu động theo trạng thái

- Original corpus path: `21_REUSE_COOKBOOK/003_menu_ng_theo_tr_ng_th_i.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-003-menu-ng-theo-tr-ng-th-i-md`
- `kind`: reuse_recipe

### Menu động theo trạng thái

#### Bài toán

Dim/add/remove command theo selection, network, playback state.

#### API/component nên tái sử dụng

- [Options Menu API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-075-options-menu-api-md)
- [Buttons API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-014-buttons-api-md)

#### Quyết định kiến trúc

Override DynInitMenuPaneL và dùng command IDs từ .hrh/.rss.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib, eikcore.lib
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không rebuild toàn menu/control bằng tay trong Draw().

#### Guide/class liên quan trong corpus

- [027 ceikmenupane](42-class-guides-026-050.md#src-12-class-guides-027-ceikmenupane-md)

#### Link chính thức

- [Options Menu API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Options_Menu_APIIndexPage.html)
- [Buttons API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Buttons_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-004-confirmation-input-v-message-query-md"></a>
## Confirmation, input và message query

- Original corpus path: `21_REUSE_COOKBOOK/004_confirmation_input_v_message_query.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-004-confirmation-input-v-message-query-md`
- `kind`: reuse_recipe

### Confirmation, input và message query

#### Bài toán

Hỏi yes/no, nhập text/number/date hoặc chọn list.

#### API/component nên tái sử dụng

- [Queries API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-090-queries-api-md)
- [Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-030-dialogs-api-md)

#### Quyết định kiến trúc

Chọn CAknQueryDialog/CAknListQueryDialog/CAknMessageQueryDialog và resource đúng type.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Phân biệt cancel với dữ liệu rỗng; kiểm tra RunLD ownership.

#### Guide/class liên quan trong corpus

- [028 caknmessagequerydialog](42-class-guides-026-050.md#src-12-class-guides-028-caknmessagequerydialog-md)
- [029 caknquerydialog](42-class-guides-026-050.md#src-12-class-guides-029-caknquerydialog-md)

#### Link chính thức

- [Queries API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Queries_APIIndexPage.html)
- [Dialogs API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Dialogs_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-005-progress-wait-v-error-note-md"></a>
## Progress, wait và error note

- Original corpus path: `21_REUSE_COOKBOOK/005_progress_wait_v_error_note.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-005-progress-wait-v-error-note-md`
- `kind`: reuse_recipe

### Progress, wait và error note

#### Bài toán

Hiển thị tiến trình async và lỗi platform-consistent.

#### API/component nên tái sử dụng

- [Notes API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-070-notes-api-md)
- [Error UI API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-036-error-ui-api-md)
- [Text Resolver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-110-text-resolver-api-md)

#### Quyết định kiến trúc

Dùng progress/wait dialog có callback cancel; resolve system error text.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib, commonengine.lib
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không block scheduler trong khi wait dialog mở.

#### Link chính thức

- [Notes API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Notes_APIIndexPage.html)
- [Error UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Error_UI_APIIndexPage.html)
- [Text Resolver API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Text_Resolver_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-006-danh-s-ch-l-n-c-search-md"></a>
## Danh sách lớn có search

- Original corpus path: `21_REUSE_COOKBOOK/006_danh_s_ch_l_n_c_search.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-006-danh-s-ch-l-n-c-search-md`
- `kind`: reuse_recipe

### Danh sách lớn có search

#### Bài toán

Listbox, model, current item và find pane.

#### API/component nên tái sử dụng

- [Lists API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-063-lists-api-md)
- [Searchfield API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-097-searchfield-api-md)
- [Scroller API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-096-scroller-api-md)

#### Quyết định kiến trúc

Dùng list model + CAknSearchField; cập nhật bằng HandleItemAdditionL/RemovalL.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không redraw toàn list mỗi key; không giữ index sau model mutation.

#### Guide/class liên quan trong corpus

- [030 caknsinglestylelistbox](42-class-guides-026-050.md#src-12-class-guides-030-caknsinglestylelistbox-md)

#### Link chính thức

- [Lists API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Lists_APIIndexPage.html)
- [Searchfield API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Searchfield_APIIndexPage.html)
- [Scroller API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Scroller_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-007-m-n-h-nh-settings-md"></a>
## Màn hình settings

- Original corpus path: `21_REUSE_COOKBOOK/007_m_n_h_nh_settings.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-007-m-n-h-nh-settings-md`
- `kind`: reuse_recipe

### Màn hình settings

#### Bài toán

Edit text, enum, slider, password trong UX chuẩn.

#### API/component nên tái sử dụng

- [Setting Pages API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-100-setting-pages-api-md)
- [Central Repository Notification Handler API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-017-central-repository-notification-handler-api-md)

#### Quyết định kiến trúc

Dùng CAknSettingItemList; persist qua CRepository hoặc model riêng.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib, centralrepository.lib
- Capabilities: Theo repository policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

UI field không phải storage; tránh write-back loop từ notification.

#### Guide/class liên quan trong corpus

- [031 caknsettingitemlist](42-class-guides-026-050.md#src-12-class-guides-031-caknsettingitemlist-md)
- [020 crepository](41-class-guides-001-025.md#src-12-class-guides-020-crepository-md)

#### Link chính thức

- [Setting Pages API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Setting_Pages_APIIndexPage.html)
- [Central Repository Notification Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Central_Repository_Notification_Handler_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-008-skin-v-icon-scalable-md"></a>
## Skin và icon scalable

- Original corpus path: `21_REUSE_COOKBOOK/008_skin_v_icon_scalable.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-008-skin-v-icon-scalable-md`
- `kind`: reuse_recipe

### Skin và icon scalable

#### Bài toán

Background, màu, MIF/SVG icon theo skin/layout.

#### API/component nên tái sử dụng

- [Skins API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-101-skins-api-md)
- [Scalable Icons API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-092-scalable-icons-api-md)
- [Fonts API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-041-fonts-api-md)

#### Quyết định kiến trúc

Dùng AknsDrawUtils/control context và AknIconUtils.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: avkon.lib; aknssrv.lib tùy class
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không hard-code màu/kích thước pixel; rescale khi SizeChanged.

#### Guide/class liên quan trong corpus

- [035 akniconutils](42-class-guides-026-050.md#src-12-class-guides-035-akniconutils-md)

#### Link chính thức

- [Skins API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Skins_APIIndexPage.html)
- [Scalable Icons API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Scalable_Icons_APIIndexPage.html)
- [Fonts API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Fonts_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-009-ch-n-file-drive-chu-n-md"></a>
## Chọn file/drive chuẩn

- Original corpus path: `21_REUSE_COOKBOOK/009_ch_n_file_drive_chu_n.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-009-ch-n-file-drive-chu-n-md`
- `kind`: reuse_recipe

### Chọn file/drive chuẩn

#### Bài toán

Mở file picker, memory selection, filename prompt.

#### API/component nên tái sử dụng

- [Common File Dialogs API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-019-common-file-dialogs-api-md)
- [Platform Environment API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-084-platform-environment-api-md)
- [Directory Localizer API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-031-directory-localizer-api-md)

#### Quyết định kiến trúc

Dùng dialog chuẩn + PathInfo/TParse; thêm MAknFileFilter.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: commonui.lib (verify), platformenv.lib
- Capabilities: Theo path truy cập

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không tự code file manager nếu chỉ cần selection.

#### Link chính thức

- [Common File Dialogs API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Common_File_Dialogs_APIIndexPage.html)
- [Platform Environment API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Platform_Environment_APIIndexPage.html)
- [Directory Localizer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Directory_Localizer_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-010-ch-n-nh-audio-video-md"></a>
## Chọn ảnh/audio/video

- Original corpus path: `21_REUSE_COOKBOOK/010_ch_n_nh_audio_video.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-010-ch-n-nh-audio-video-md`
- `kind`: reuse_recipe

### Chọn ảnh/audio/video

#### Bài toán

Cho user chọn media theo type.

#### API/component nên tái sử dụng

- [Media Fetch API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-066-media-fetch-api-md)

#### Quyết định kiến trúc

Dùng MGFetch/media picker thay vì crawl gallery.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: Verify SDK example
- Capabilities: ReadUserData/file policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Xử lý cancel, nhiều selection và file biến mất.

#### Link chính thức

- [Media Fetch API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Media_Fetch_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-011-ng-d-n-h-th-ng-v-disk-space-md"></a>
## Đường dẫn hệ thống và disk space

- Original corpus path: `21_REUSE_COOKBOOK/011_ng_d_n_h_th_ng_v_disk_space.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-011-ng-d-n-h-th-ng-v-disk-space-md`
- `kind`: reuse_recipe

### Đường dẫn hệ thống và disk space

#### Bài toán

Lưu cache/data đúng drive/path và tránh disk full.

#### API/component nên tái sử dụng

- [Platform Environment API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-084-platform-environment-api-md)
- [System Utils API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-108-system-utils-api-md)

#### Quyết định kiến trúc

PathInfo + RFs/TParse; SysUtil critical disk check trước ghi lớn.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: platformenv.lib, sysutil.lib, efsrv.lib
- Capabilities: Theo path

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không hard-code C:/E: hoặc folder tiếng Anh.

#### Guide/class liên quan trong corpus

- [071 pathinfo](43-class-guides-051-075.md#src-12-class-guides-071-pathinfo-md)
- [072 sysutil](43-class-guides-051-075.md#src-12-class-guides-072-sysutil-md)
- [010 rfs](41-class-guides-001-025.md#src-12-class-guides-010-rfs-md)

#### Link chính thức

- [Platform Environment API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Platform_Environment_APIIndexPage.html)
- [System Utils API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/System_Utils_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-012-feature-version-gating-md"></a>
## Feature/version gating

- Original corpus path: `21_REUSE_COOKBOOK/012_feature_version_gating.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-012-feature-version-gating-md`
- `kind`: reuse_recipe

### Feature/version gating

#### Bài toán

Bật chức năng chỉ khi firmware/hardware hỗ trợ.

#### API/component nên tái sử dụng

- [Feature Discovery API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-039-feature-discovery-api-md)
- [Version Info API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-113-version-info-api-md)

#### Quyết định kiến trúc

Feature flag trước; version chỉ là fallback.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: featdiscovery.lib/featmgr.lib, platformenv.lib
- Capabilities: Thường None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không map model name thành feature set.

#### Guide/class liên quan trong corpus

- [062 featuremanager](43-class-guides-051-075.md#src-12-class-guides-062-featuremanager-md)
- [075 cversioninfo](43-class-guides-051-075.md#src-12-class-guides-075-cversioninfo-md)

#### Link chính thức

- [Feature Discovery API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Feature_Discovery_APIIndexPage.html)
- [Version Info API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Version_Info_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-013-m-file-b-ng-app-h-th-ng-md"></a>
## Mở file bằng app hệ thống

- Original corpus path: `21_REUSE_COOKBOOK/013_m_file_b_ng_app_h_th_ng.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-013-m-file-b-ng-app-h-th-ng-md`
- `kind`: reuse_recipe

### Mở file bằng app hệ thống

#### Bài toán

Open/view/edit content có MIME handler sẵn.

#### API/component nên tái sử dụng

- [Document Handler API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-032-document-handler-api-md)

#### Quyết định kiến trúc

Dùng CDocumentHandler và xử lý no-handler/embedded exit.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: commonui.lib
- Capabilities: Theo file/operation

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không nhúng PDF/image/audio viewer riêng nếu system handler đủ.

#### Guide/class liên quan trong corpus

- [063 cdocumenthandler](43-class-guides-051-075.md#src-12-class-guides-063-cdocumenthandler-md)

#### Link chính thức

- [Document Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Document_Handler_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-014-send-via-email-mms-bluetooth-md"></a>
## Send via email/MMS/Bluetooth

- Original corpus path: `21_REUSE_COOKBOOK/014_send_via_email_mms_bluetooth.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-014-send-via-email-mms-bluetooth-md`
- `kind`: reuse_recipe

### Send via email/MMS/Bluetooth

#### Bài toán

Chia sẻ text/file qua service đã cài.

#### API/component nên tái sử dụng

- [Send UI API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-098-send-ui-api-md)

#### Quyết định kiến trúc

CMessageData + CSendUi, để platform query service.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: sendui.lib
- Capabilities: Theo attachment/data

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không tự hard-code transport menu/account UI.

#### Guide/class liên quan trong corpus

- [069 csendui](43-class-guides-051-075.md#src-12-class-guides-069-csendui-md)
- [070 cmessagedata](43-class-guides-051-075.md#src-12-class-guides-070-cmessagedata-md)

#### Link chính thức

- [Send UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Send_UI_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-015-nh-ng-trang-web-md"></a>
## Nhúng trang web

- Original corpus path: `21_REUSE_COOKBOOK/015_nh_ng_trang_web.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-015-nh-ng-trang-web-md`
- `kind`: reuse_recipe

### Nhúng trang web

#### Bài toán

Render HTML hoặc web UI trong control.

#### API/component nên tái sử dụng

- [Browser Control API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-012-browser-control-api-md)

#### Quyết định kiến trúc

Tạo browser control và implement softkey/link/special-load observers.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: Verify Browser Control SDK metadata
- Capabilities: NetworkServices + source-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Engine cũ không phải Chromium; test JS/TLS thật trên E72.

#### Link chính thức

- [Browser Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Browser_Control_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-016-download-c-queue-resume-progress-md"></a>
## Download có queue/resume/progress

- Original corpus path: `21_REUSE_COOKBOOK/016_download_c_queue_resume_progress.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-016-download-c-queue-resume-progress-md`
- `kind`: reuse_recipe

### Download có queue/resume/progress

#### Bài toán

Tải file dài, quản lý trạng thái và UI downloads.

#### API/component nên tái sử dụng

- [Download Manager Engine API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-033-download-manager-engine-api-md)
- [Download Manager UI Library API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-034-download-manager-ui-library-api-md)

#### Quyết định kiến trúc

Tái sử dụng RHttpDownloadMgr/UI library khi semantics phù hợp.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: Verify local SDK
- Capabilities: NetworkServices + file path

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không vừa dùng manager vừa tự ghi cùng destination.

#### Link chính thức

- [Download Manager Engine API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Download_Manager_Engine_APIIndexPage.html)
- [Download Manager UI Library API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Download_Manager_UI_Library_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-017-ch-n-destination-iap-fp2-md"></a>
## Chọn destination/IAP FP2

- Original corpus path: `21_REUSE_COOKBOOK/017_ch_n_destination_iap_fp2.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-017-ch-n-destination-iap-fp2-md`
- `kind`: reuse_recipe

### Chọn destination/IAP FP2

#### Bài toán

Liệt kê destination/connection method hiện có.

#### API/component nên tái sử dụng

- [Connection Settings API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-022-connection-settings-api-md)
- [Connection Settings UI API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-023-connection-settings-ui-api-md)

#### Quyết định kiến trúc

RCmManager cho model; UI API cho selector chuẩn.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: cmmanager.lib; UI lib verify
- Capabilities: ReadDeviceData cho private attrs; operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không dùng Access Point Engine deprecated cho code mới.

#### Link chính thức

- [Connection Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_APIIndexPage.html)
- [Connection Settings UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_UI_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-018-theo-d-i-data-connection-md"></a>
## Theo dõi data connection

- Original corpus path: `21_REUSE_COOKBOOK/018_theo_d_i_data_connection.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-018-theo-d-i-data-connection-md`
- `kind`: reuse_recipe

### Theo dõi data connection

#### Bài toán

Biết connection tạo/xoá/thay bearer/attributes.

#### API/component nên tái sử dụng

- [Connection Monitor Server API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-021-connection-monitor-server-api-md)

#### Quyết định kiến trúc

RConnectionMonitor + observer events.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: connmon.lib
- Capabilities: NetworkServices/operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Connection up không đồng nghĩa Internet reachable.

#### Guide/class liên quan trong corpus

- [066 rconnectionmonitor](43-class-guides-051-075.md#src-12-class-guides-066-rconnectionmonitor-md)

#### Link chính thức

- [Connection Monitor Server API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Monitor_Server_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-019-tcp-client-md"></a>
## TCP client

- Original corpus path: `21_REUSE_COOKBOOK/019_tcp_client.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-019-tcp-client-md`
- `kind`: reuse_recipe

### TCP client

#### Bài toán

Kết nối server, read/write async.

#### API/component nên tái sử dụng

- [Connection Settings API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-022-connection-settings-api-md)

#### Quyết định kiến trúc

RSocketServ/RConnection/RHostResolver/RSocket từ Symbian socket APIs; dùng destination selection phía S60.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: esock.lib, insock.lib
- Capabilities: NetworkServices

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Một outstanding request mỗi AO; buffer phải sống tới completion.

#### Guide/class liên quan trong corpus

- [SOCKETS DNS TCP UDP](13-deep-guides.md#src-14-deep-guides-sockets-dns-tcp-udp-md)
- [036 rsocketserv](42-class-guides-026-050.md#src-12-class-guides-036-rsocketserv-md)
- [037 rsocket](42-class-guides-026-050.md#src-12-class-guides-037-rsocket-md)
- [038 rconnection](42-class-guides-026-050.md#src-12-class-guides-038-rconnection-md)
- [039 rhostresolver](42-class-guides-026-050.md#src-12-class-guides-039-rhostresolver-md)

#### Link chính thức

- [Connection Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-020-udp-client-server-md"></a>
## UDP client/server

- Original corpus path: `21_REUSE_COOKBOOK/020_udp_client_server.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-020-udp-client-server-md`
- `kind`: reuse_recipe

### UDP client/server

#### Bài toán

Datagram send/receive.

#### API/component nên tái sử dụng

- [Connection Settings API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-022-connection-settings-api-md)

#### Quyết định kiến trúc

RSocket KSockDatagram/KProtocolInetUdp; optional RConnection.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: esock.lib, insock.lib
- Capabilities: NetworkServices

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Xử lý packet boundary, truncation và no delivery guarantee.

#### Guide/class liên quan trong corpus

- [SOCKETS DNS TCP UDP](13-deep-guides.md#src-14-deep-guides-sockets-dns-tcp-udp-md)
- [036 rsocketserv](42-class-guides-026-050.md#src-12-class-guides-036-rsocketserv-md)
- [037 rsocket](42-class-guides-026-050.md#src-12-class-guides-037-rsocket-md)
- [038 rconnection](42-class-guides-026-050.md#src-12-class-guides-038-rconnection-md)

#### Link chính thức

- [Connection Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-021-http-client-md"></a>
## HTTP client

- Original corpus path: `21_REUSE_COOKBOOK/021_http_client.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-021-http-client-md`
- `kind`: reuse_recipe

### HTTP client

#### Bài toán

GET/POST, headers, streaming response.

#### API/component nên tái sử dụng

- [Connection Settings API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-022-connection-settings-api-md)

#### Quyết định kiến trúc

RHTTPSession/RHTTPTransaction + MHTTPTransactionCallback; attach connection nếu cần.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: http.lib, esock.lib, insock.lib
- Capabilities: NetworkServices

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

ReleaseData mỗi body chunk; không buffer file lớn vào RAM.

#### Guide/class liên quan trong corpus

- [HTTP FRAMEWORK FP2](13-deep-guides.md#src-14-deep-guides-http-framework-fp2-md)
- [041 rhttpsession](42-class-guides-026-050.md#src-12-class-guides-041-rhttpsession-md)
- [042 rhttptransaction](42-class-guides-026-050.md#src-12-class-guides-042-rhttptransaction-md)
- [043 mhttptransactioncallback](42-class-guides-026-050.md#src-12-class-guides-043-mhttptransactioncallback-md)
- [044 curi8](42-class-guides-026-050.md#src-12-class-guides-044-curi8-md)

#### Link chính thức

- [Connection Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Connection_Settings_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-022-bluetooth-service-link-md"></a>
## Bluetooth service/link

- Original corpus path: `21_REUSE_COOKBOOK/022_bluetooth_service_link.md`
- Consolidated reference: `61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-022-bluetooth-service-link-md`
- `kind`: reuse_recipe

### Bluetooth service/link

#### Bài toán

Kết nối thiết bị local qua BT.

#### API/component nên tái sử dụng

- [Feature Discovery API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-039-feature-discovery-api-md)

#### Quyết định kiến trúc

Dùng Bluetooth socket/service APIs và plugin/device docs; gate feature.

Giữ business/domain layer của app độc lập; bọc API Symbian trong adapter/service có ownership
và async state machine rõ ràng. Nhờ vậy có thể mock trên emulator và thay implementation khi
firmware/device thiếu component.

#### Luồng triển khai

1. Mở manual của từng API ở trên và chọn exact entry class.
2. Xác minh header trong `%EPOCROOT%epoc32\include`.
3. Tìm SDK example/`.mmp` để chốt import library.
4. Chốt capability theo exact methods.
5. Viết compile probe nhỏ trước khi gắn vào app lớn.
6. Bọc callback/Active Object thành state machine có cancel/timeout.
7. Test cả success, cancel, unsupported, permission, low-memory và device-specific path.

#### Build/security dự kiến

- Libraries: bluetooth.lib và libs theo profile
- Capabilities: LocalServices

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không assume RFCOMM/service UUID có sẵn; discovery async.

#### Guide/class liên quan trong corpus

- [067 cbluetoothsynchronouslink](43-class-guides-051-075.md#src-12-class-guides-067-cbluetoothsynchronouslink-md)
- [068 rbtphysicallinkadapter](43-class-guides-051-075.md#src-12-class-guides-068-rbtphysicallinkadapter-md)

#### Link chính thức

- [Feature Discovery API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Feature_Discovery_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
