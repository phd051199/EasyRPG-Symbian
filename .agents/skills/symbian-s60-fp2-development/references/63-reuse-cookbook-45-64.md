# Reuse cookbook recipes 45–64

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-21-reuse-cookbook-045-vibration-v-lights-md"></a>
## Vibration và lights

- Original corpus path: `21_REUSE_COOKBOOK/045_vibration_v_lights.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-045-vibration-v-lights-md`
- `kind`: reuse_recipe

### Vibration và lights

#### Bài toán

Feedback haptic/LED/backlight.

#### API/component nên tái sử dụng

- [Vibra API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-114-vibra-api-md)
- [Light API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-062-light-api-md)

#### Quyết định kiến trúc

HWRM clients + feature/device policy.

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

- Libraries: hwrmvibraclient.lib, hwrmlightclient.lib
- Capabilities: Device policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Tôn trọng profile/power; stop khi background.

#### Guide/class liên quan trong corpus

- [064 chwrmlight](43-class-guides-051-075.md#src-12-class-guides-064-chwrmlight-md)
- [065 chwrmvibra](43-class-guides-051-075.md#src-12-class-guides-065-chwrmvibra-md)

#### Link chính thức

- [Vibra API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Vibra_APIIndexPage.html)
- [Light API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Light_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-046-keylock-keyguard-md"></a>
## Keylock/keyguard

- Original corpus path: `21_REUSE_COOKBOOK/046_keylock_keyguard.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-046-keylock-keyguard-md`
- `kind`: reuse_recipe

### Keylock/keyguard

#### Bài toán

Query/change keylock qua platform API.

#### API/component nên tái sử dụng

- [Keylock API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-055-keylock-api-md)

#### Quyết định kiến trúc

Dùng Akn keylock utility; check security/user expectations.

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
- Capabilities: Operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không unlock silently hoặc fight system policy.

#### Link chính thức

- [Keylock API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Keylock_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-047-auto-start-trusted-component-md"></a>
## Auto-start trusted component

- Original corpus path: `21_REUSE_COOKBOOK/047_auto_start_trusted_component.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-047-auto-start-trusted-component-md`
- `kind`: reuse_recipe

### Auto-start trusted component

#### Bài toán

Đăng ký startup item khi installer/policy cho phép.

#### API/component nên tái sử dụng

- [Startup List Management API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-105-startup-list-management-api-md)

#### Quyết định kiến trúc

Dùng startup resource/package mechanism, không sửa file list trực tiếp.

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

- Libraries: Installer metadata
- Capabilities: Trusted signing/capability requirements

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Auto-start là power/privacy-sensitive; failure path phải an toàn.

#### Link chính thức

- [Startup List Management API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Startup_List_Management_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-048-ecom-style-plugin-md"></a>
## ECom-style plugin

- Original corpus path: `21_REUSE_COOKBOOK/048_ecom_style_plugin.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-048-ecom-style-plugin-md`
- `kind`: reuse_recipe

### ECom-style plugin

#### Bài toán

Load implementation theo interface UID.

#### API/component nên tái sử dụng

- [Screen Saver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-095-screen-saver-api-md)
- [Browser Plug-in API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-013-browser-plug-in-api-md)

#### Quyết định kiến trúc

Dùng ECom contract/registration; chỉ chọn API group đúng service.

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

- Libraries: ecom.lib + service libs
- Capabilities: Theo service

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

FinalClose sau khi mọi implementation chết.

#### Guide/class liên quan trong corpus

- [061 recomsession](43-class-guides-051-075.md#src-12-class-guides-061-recomsession-md)

#### Link chính thức

- [Screen Saver API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Screen_Saver_APIIndexPage.html)
- [Browser Plug-in API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Browser_Plug-in_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-049-invoke-service-b-ng-aiw-md"></a>
## Invoke service bằng AIW

- Original corpus path: `21_REUSE_COOKBOOK/049_invoke_service_b_ng_aiw.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-049-invoke-service-b-ng-aiw-md`
- `kind`: reuse_recipe

### Invoke service bằng AIW

#### Bài toán

Context action/provider discovery.

#### API/component nên tái sử dụng

- [AIW Criteria API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-001-aiw-criteria-api-md)
- [AIW Generic Parameter API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-002-aiw-generic-parameter-api-md)
- [AIW Service Handler API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-003-aiw-service-handler-api-md)

#### Quyết định kiến trúc

Criteria + generic params + service handler.

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

- Libraries: AIW service libs verify
- Capabilities: Theo provider

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Fallback khi không provider; validate variant types.

#### Link chính thức

- [AIW Criteria API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AIW_Criteria_APIIndexPage.html)
- [AIW Generic Parameter API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AIW_Generic_Parameter_APIIndexPage.html)
- [AIW Service Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/AIW_Service_Handler_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-050-soap-web-service-client-md"></a>
## SOAP/web service client

- Original corpus path: `21_REUSE_COOKBOOK/050_soap_web_service_client.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-050-soap-web-service-client-md`
- `kind`: reuse_recipe

### SOAP/web service client

#### Bài toán

Service description, credentials, transport properties.

#### API/component nên tái sử dụng

- [Web Service Connection API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-117-web-service-connection-api-md)
- [Web Service Description API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-118-web-service-description-api-md)
- [Web Service Manager API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-119-web-service-manager-api-md)

#### Quyết định kiến trúc

Dùng Sen web-service stack nếu endpoint tương thích.

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

- Libraries: Sen libs verify
- Capabilities: NetworkServices + credential policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Stack cũ; test TLS/SOAP interoperability thật.

#### Link chính thức

- [Web Service Connection API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Web_Service_Connection_APIIndexPage.html)
- [Web Service Description API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Web_Service_Description_APIIndexPage.html)
- [Web Service Manager API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Web_Service_Manager_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-051-xml-model-reader-md"></a>
## XML model/reader

- Original corpus path: `21_REUSE_COOKBOOK/051_xml_model_reader.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-051-xml-model-reader-md`
- `kind`: reuse_recipe

### XML model/reader

#### Bài toán

Build/read XML fragments for Sen/web service.

#### API/component nên tái sử dụng

- [XML Extensions API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-121-xml-extensions-api-md)

#### Quyết định kiến trúc

Dùng Sen XML abstractions khi làm việc cùng web-service stack.

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

- Libraries: Sen/XML libs verify
- Capabilities: None/data-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không mix ownership giữa DOM fragments/elements mù quáng.

#### Link chính thức

- [XML Extensions API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/XML_Extensions_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-052-localized-error-message-md"></a>
## Localized error message

- Original corpus path: `21_REUSE_COOKBOOK/052_localized_error_message.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-052-localized-error-message-md`
- `kind`: reuse_recipe

### Localized error message

#### Bài toán

Map error code và show correct note.

#### API/component nên tái sử dụng

- [Text Resolver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-110-text-resolver-api-md)
- [Error UI API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-036-error-ui-api-md)

#### Quyết định kiến trúc

Resolve text; log code; choose error UI wrapper.

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

- Libraries: commonengine.lib
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không swallow error hoặc dùng text làm program logic.

#### Guide/class liên quan trong corpus

- [073 ctextresolver](43-class-guides-051-075.md#src-12-class-guides-073-ctextresolver-md)

#### Link chính thức

- [Text Resolver API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Text_Resolver_APIIndexPage.html)
- [Error UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Error_UI_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-053-validate-phone-number-md"></a>
## Validate phone number

- Original corpus path: `21_REUSE_COOKBOOK/053_validate_phone_number.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-053-validate-phone-number-md`
- `kind`: reuse_recipe

### Validate phone number

#### Bài toán

Parse/check phone-number syntax.

#### API/component nên tái sử dụng

- [Phone Parser API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-076-phone-parser-api-md)

#### Quyết định kiến trúc

CommonPhoneParser trước dial/store action.

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

- Libraries: commonengine/phone parser lib verify
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Parsing không chứng minh number reachable.

#### Link chính thức

- [Phone Parser API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phone_Parser_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-054-simple-notes-text-storage-md"></a>
## Simple notes/text storage

- Original corpus path: `21_REUSE_COOKBOOK/054_simple_notes_text_storage.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-054-simple-notes-text-storage-md`
- `kind`: reuse_recipe

### Simple notes/text storage

#### Bài toán

Reuse Notepad data/edit behavior.

#### API/component nên tái sử dụng

- [Notepad Library API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-069-notepad-library-api-md)

#### Quyết định kiến trúc

Dùng Npd API cho supported note/template/text use-case.

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

- Libraries: Verify SDK
- Capabilities: User data policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không chọn nếu app cần schema/search/sync riêng.

#### Link chính thức

- [Notepad Library API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Notepad_Library_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-055-ocr-md"></a>
## OCR

- Original corpus path: `21_REUSE_COOKBOOK/055_ocr.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-055-ocr-md`
- `kind`: reuse_recipe

### OCR

#### Bài toán

Nhận dạng text từ image qua platform OCR server.

#### API/component nên tái sử dụng

- [Optical Character Recognition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-074-optical-character-recognition-api-md)

#### Quyết định kiến trúc

OCR server/client nếu firmware có implementation.

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

- Libraries: OCR libs verify
- Capabilities: UserEnvironment/file policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Gate feature; memory/accuracy/device availability vary.

#### Link chính thức

- [Optical Character Recognition API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Optical_Character_Recognition_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-056-open-flash-content-md"></a>
## Open Flash content

- Original corpus path: `21_REUSE_COOKBOOK/056_open_flash_content.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-056-open-flash-content-md`
- `kind`: reuse_recipe

### Open Flash content

#### Bài toán

Launch platform Flash viewer.

#### API/component nên tái sử dụng

- [Flash Viewer Framework API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-040-flash-viewer-framework-api-md)

#### Quyết định kiến trúc

Dùng Flash viewer framework, không viết SWF runtime.

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

- Libraries: Verify SDK
- Capabilities: File policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Flash availability/version device-specific.

#### Link chính thức

- [Flash Viewer Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Flash_Viewer_Framework_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-057-screen-saver-extension-md"></a>
## Screen saver extension

- Original corpus path: `21_REUSE_COOKBOOK/057_screen_saver_extension.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-057-screen-saver-extension-md`
- `kind`: reuse_recipe

### Screen saver extension

#### Bài toán

Implement platform screen-saver plugin.

#### API/component nên tái sử dụng

- [Screen Saver API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-095-screen-saver-api-md)

#### Quyết định kiến trúc

Follow plugin interface, UIDs and lifecycle.

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

- Libraries: ECom/screensaver libs
- Capabilities: Signing/device policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Power constraints; no heavy work/network loop.

#### Link chính thức

- [Screen Saver API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Screen_Saver_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-058-instant-messaging-provider-client-md"></a>
## Instant messaging provider/client

- Original corpus path: `21_REUSE_COOKBOOK/058_instant_messaging_provider_client.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-058-instant-messaging-provider-client-md`
- `kind`: reuse_recipe

### Instant messaging provider/client

#### Bài toán

Integrate IM service/application launch.

#### API/component nên tái sử dụng

- [Instant Messaging API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-045-instant-messaging-api-md)
- [IM Application Launch API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-046-im-application-launch-api-md)

#### Quyết định kiến trúc

Use ECom client/provider contracts.

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

- Libraries: IM libs verify
- Capabilities: NetworkServices/user data as operation requires

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Implementation may not exist on target firmware.

#### Link chính thức

- [Instant Messaging API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Instant_Messaging_APIIndexPage.html)
- [IM Application Launch API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/IM_Application_Launch_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-059-predictive-input-custom-dictionary-md"></a>
## Predictive input/custom dictionary

- Original corpus path: `21_REUSE_COOKBOOK/059_predictive_input_custom_dictionary.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-059-predictive-input-custom-dictionary-md`
- `kind`: reuse_recipe

### Predictive input/custom dictionary

#### Bài toán

Interact with PTI engine/input languages.

#### API/component nên tái sử dụng

- [Predictive Input PTI Engine API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-089-predictive-input-pti-engine-api-md)
- [Input Language Info API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-053-input-language-info-api-md)

#### Quyết định kiến trúc

Use PTI language/mapping/dictionary interfaces.

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

- Libraries: PTI libs verify
- Capabilities: None/user data policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Language databases vary; do not assume Latin keypad mapping.

#### Link chính thức

- [Predictive Input PTI Engine API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Predictive_Input_PTI_Engine_APIIndexPage.html)
- [Input Language Info API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Input_Language_Info_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-060-embedded-server-application-md"></a>
## Embedded server application

- Original corpus path: `21_REUSE_COOKBOOK/060_embedded_server_application.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-060-embedded-server-application-md`
- `kind`: reuse_recipe

### Embedded server application

#### Bài toán

Expose document/service UI to another app.

#### API/component nên tái sử dụng

- [Server Application API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-099-server-application-api-md)

#### Quyết định kiến trúc

Use CAknServerApp-style contract instead of custom IPC+UI if fit.

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

- Libraries: avkon/apparc service libs
- Capabilities: Operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Session/document lifetime and client exit callback matter.

#### Link chính thức

- [Server Application API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Server_Application_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-061-observe-global-window-server-events-md"></a>
## Observe global/window-server events

- Original corpus path: `21_REUSE_COOKBOOK/061_observe_global_window_server_events.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-061-observe-global-window-server-events-md`
- `kind`: reuse_recipe

### Observe global/window-server events

#### Bài toán

React to selected WS events outside normal control stack.

#### API/component nên tái sử dụng

- [Window Server Event Observer API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-120-window-server-event-observer-api-md)

#### Quyết định kiến trúc

Register observer through AppUi/framework API.

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

- Libraries: avkon/ws32 libs
- Capabilities: None/operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Callback must be fast; avoid duplicate normal control handling.

#### Link chính thức

- [Window Server Event Observer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Window_Server_Event_Observer_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-062-localized-folder-names-md"></a>
## Localized folder names

- Original corpus path: `21_REUSE_COOKBOOK/062_localized_folder_names.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-062-localized-folder-names-md`
- `kind`: reuse_recipe

### Localized folder names

#### Bài toán

Display human-readable directory names.

#### API/component nên tái sử dụng

- [Directory Localizer API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-031-directory-localizer-api-md)

#### Quyết định kiến trúc

Use CDirectoryLocalizer instead of translating path components yourself.

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

- Libraries: platformenv/common UI lib verify
- Capabilities: None

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Keep filesystem path separate from display name.

#### Link chính thức

- [Directory Localizer API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Directory_Localizer_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-063-standard-sounds-md"></a>
## Standard sounds

- Original corpus path: `21_REUSE_COOKBOOK/063_standard_sounds.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-063-standard-sounds-md`
- `kind`: reuse_recipe

### Standard sounds

#### Bài toán

Play key/UI sounds consistent with system.

#### API/component nên tái sử dụng

- [Sounds API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-103-sounds-api-md)

#### Quyết định kiến trúc

Use Akn sound system/info rather than raw audio clips for UI feedback.

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

Respect profile/silent mode.

#### Link chính thức

- [Sounds API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Sounds_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-064-global-notification-md"></a>
## Global notification

- Original corpus path: `21_REUSE_COOKBOOK/064_global_notification.md`
- Consolidated reference: `63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-064-global-notification-md`
- `kind`: reuse_recipe

### Global notification

#### Bài toán

Show global note/query/progress outside ordinary dialog context.

#### API/component nên tái sử dụng

- [Notifiers API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-071-notifiers-api-md)

#### Quyết định kiến trúc

Use global notifier APIs; keep interaction minimal.

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

- Libraries: avkon/notifier libs
- Capabilities: Operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Do not spam or create modal deadlocks.

#### Link chính thức

- [Notifiers API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Notifiers_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
