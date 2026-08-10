# Reuse cookbook recipes 23–44

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-21-reuse-cookbook-023-audio-playback-md"></a>
## Audio playback

- Original corpus path: `21_REUSE_COOKBOOK/023_audio_playback.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-023-audio-playback-md`
- `kind`: reuse_recipe

### Audio playback

#### Bài toán

Phát file/URL audio bằng utility platform.

#### API/component nên tái sử dụng

- [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Quyết định kiến trúc

CMdaAudioPlayerUtility cho player; Audio Effects chỉ thêm EQ/reverb/etc.

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

- Libraries: mediaclientaudio.lib + effect libs
- Capabilities: UserEnvironment/device-policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Chờ init callback trước Play; codec/URL support phụ thuộc máy.

#### Guide/class liên quan trong corpus

- [045 cmdaaudioplayerutility](42-class-guides-026-050.md#src-12-class-guides-045-cmdaaudioplayerutility-md)

#### Link chính thức

- [Audio Effects API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Audio_Effects_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-024-audio-recording-md"></a>
## Audio recording

- Original corpus path: `21_REUSE_COOKBOOK/024_audio_recording.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-024-audio-recording-md`
- `kind`: reuse_recipe

### Audio recording

#### Bài toán

Record microphone, state callbacks và file output.

#### API/component nên tái sử dụng

- [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)

#### Quyết định kiến trúc

CMdaAudioRecorderUtility; effects nếu supported.

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

- Libraries: mediaclientaudio.lib
- Capabilities: UserEnvironment và policy cụ thể

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Stop/close khi background; test format/bitrate trên device.

#### Guide/class liên quan trong corpus

- [046 cmdaaudiorecorderutility](42-class-guides-026-050.md#src-12-class-guides-046-cmdaaudiorecorderutility-md)

#### Link chính thức

- [Audio Effects API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Audio_Effects_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-025-video-playback-md"></a>
## Video playback

- Original corpus path: `21_REUSE_COOKBOOK/025_video_playback.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-025-video-playback-md`
- `kind`: reuse_recipe

### Video playback

#### Bài toán

Open/prepare/play video trong window.

#### API/component nên tái sử dụng

- [ROP Custom Commands API](24-api-catalog-091-121.md#src-10-s60-quick-reference-apis-091-rop-custom-commands-api-md)

#### Quyết định kiến trúc

CVideoPlayerUtility + Window Server; ROP custom commands chỉ legacy/special.

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

- Libraries: mediaclientvideo.lib, ws32.lib
- Capabilities: UserEnvironment/device-policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Method như SetVideoRectL có thể khác edition; đối chiếu FP2 header.

#### Guide/class liên quan trong corpus

- [047 cvideoplayerutility](42-class-guides-026-050.md#src-12-class-guides-047-cvideoplayerutility-md)

#### Link chính thức

- [ROP Custom Commands API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/ROP_Custom_Commands_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-026-camera-capture-md"></a>
## Camera capture

- Original corpus path: `21_REUSE_COOKBOOK/026_camera_capture.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-026-camera-capture-md`
- `kind`: reuse_recipe

### Camera capture

#### Bài toán

Reserve camera, viewfinder, still/video capture.

#### API/component nên tái sử dụng

- [Feature Discovery API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-039-feature-discovery-api-md)

#### Quyết định kiến trúc

CCamera/ECam; use advanced settings plugin only if installed.

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

- Libraries: ecam.lib
- Capabilities: UserEnvironment/device policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Reserve/PowerOn async; release camera on background.

#### Guide/class liên quan trong corpus

- [048 ccamera](42-class-guides-026-050.md#src-12-class-guides-048-ccamera-md)

#### Link chính thức

- [Feature Discovery API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Feature_Discovery_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-027-decode-encode-image-md"></a>
## Decode/encode image

- Original corpus path: `21_REUSE_COOKBOOK/027_decode_encode_image.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-027-decode-encode-image-md`
- `kind`: reuse_recipe

### Decode/encode image

#### Bài toán

JPEG/PNG/GIF decode, thumbnail và resize.

#### API/component nên tái sử dụng

- [Images API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-047-images-api-md)

#### Quyết định kiến trúc

Dùng CImageDecoder/CImageEncoder (Symbian ICL); S60 Images API là UI image control.

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

- Libraries: imageconversion.lib, fbscli.lib
- Capabilities: Theo file path

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không tự viết JPEG/PNG codec.

#### Guide/class liên quan trong corpus

- [049 cimagedecoder](42-class-guides-026-050.md#src-12-class-guides-049-cimagedecoder-md)
- [050 cimageencoder](42-class-guides-026-050.md#src-12-class-guides-050-cimageencoder-md)
- [034 cfbsbitmap](42-class-guides-026-050.md#src-12-class-guides-034-cfbsbitmap-md)

#### Link chính thức

- [Images API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Images_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-028-c-s-a-exif-md"></a>
## Đọc/sửa EXIF

- Original corpus path: `21_REUSE_COOKBOOK/028_c_s_a_exif.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-028-c-s-a-exif-md`
- `kind`: reuse_recipe

### Đọc/sửa EXIF

#### Bài toán

Metadata camera, orientation, GPS tags.

#### API/component nên tái sử dụng

- [Exif API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-037-exif-api-md)

#### Quyết định kiến trúc

CExifRead/CExifModify; validate tag type/IFD.

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

- Libraries: Verify class page/SDK
- Capabilities: Theo file path

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không patch APP1 offsets thủ công.

#### Link chính thức

- [Exif API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Exif_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-029-eq-reverb-spatial-audio-md"></a>
## EQ/reverb/spatial audio

- Original corpus path: `21_REUSE_COOKBOOK/029_eq_reverb_spatial_audio.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-029-eq-reverb-spatial-audio-md`
- `kind`: reuse_recipe

### EQ/reverb/spatial audio

#### Bài toán

Điều khiển effect có sẵn trên output/player.

#### API/component nên tái sử dụng

- [Audio Effects API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-008-audio-effects-api-md)
- [Audio Effects Presets API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-009-audio-effects-presets-api-md)

#### Quyết định kiến trúc

Create effect for supported utility/stream; observe enable/enforce changes.

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

- Libraries: Effect-specific libs
- Capabilities: Device-policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Feature/effect có thể unsupported dù header compile.

#### Link chính thức

- [Audio Effects API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Audio_Effects_APIIndexPage.html)
- [Audio Effects Presets API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Audio_Effects_Presets_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-030-li-t-k-th-vi-n-media-md"></a>
## Liệt kê thư viện media

- Original corpus path: `21_REUSE_COOKBOOK/030_li_t_k_th_vi_n_media.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-030-li-t-k-th-vi-n-media-md`
- `kind`: reuse_recipe

### Liệt kê thư viện media

#### Bài toán

Filter/sort/group media item và metadata.

#### API/component nên tái sử dụng

- [Content Listing Framework API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-024-content-listing-framework-api-md)

#### Quyết định kiến trúc

CLF engine/model thay vì recursive filesystem scan.

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
- Capabilities: ReadUserData/file policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Refresh async; item metadata có thể lazy/missing.

#### Link chính thức

- [Content Listing Framework API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Content_Listing_Framework_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-031-drm-protected-content-md"></a>
## DRM protected content

- Original corpus path: `21_REUSE_COOKBOOK/031_drm_protected_content.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-031-drm-protected-content-md`
- `kind`: reuse_recipe

### DRM protected content

#### Bài toán

Check rights, play protected audio, handle expiry.

#### API/component nên tái sử dụng

- [DRM Helper API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-027-drm-helper-api-md)
- [DRM License Checker API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-028-drm-license-checker-api-md)
- [OMA DRM CAF Agent API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-072-oma-drm-caf-agent-api-md)
- [DRM Audio Player API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-026-drm-audio-player-api-md)

#### Quyết định kiến trúc

Dùng CAF/DRM helper/agent, không tự giải mã hay bypass rights.

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

- Libraries: DRM/CAF libs per class
- Capabilities: DRM/data operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không assume rights object transferable; handle no rights gracefully.

#### Link chính thức

- [DRM Helper API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/DRM_Helper_APIIndexPage.html)
- [DRM License Checker API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/DRM_License_Checker_APIIndexPage.html)
- [OMA DRM CAF Agent API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/OMA_DRM_CAF_Agent_APIIndexPage.html)
- [DRM Audio Player API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/DRM_Audio_Player_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-032-contacts-crud-md"></a>
## Contacts CRUD

- Original corpus path: `21_REUSE_COOKBOOK/032_contacts_crud.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-032-contacts-crud-md`
- `kind`: reuse_recipe

### Contacts CRUD

#### Bài toán

Read/search/edit contact database.

#### API/component nên tái sử dụng

- [Phonebook Data Management API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-078-phonebook-data-management-api-md)

#### Quyết định kiến trúc

Ưu tiên underlying Contact Model hoặc S60 wrapper theo UI integration; API Phonebook legacy vẫn có trên FP2.

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

- Libraries: cntmodel.lib / phonebook libs
- Capabilities: ReadUserData, WriteUserData

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Handle lock, contact field content type, deprecated path for future ports.

#### Guide/class liên quan trong corpus

- [053 ccontactdatabase](43-class-guides-051-075.md#src-12-class-guides-053-ccontactdatabase-md)
- [054 ccontactitem](43-class-guides-051-075.md#src-12-class-guides-054-ccontactitem-md)

#### Link chính thức

- [Phonebook Data Management API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_Data_Management_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-033-contact-picker-md"></a>
## Contact picker

- Original corpus path: `21_REUSE_COOKBOOK/033_contact_picker.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-033-contact-picker-md`
- `kind`: reuse_recipe

### Contact picker

#### Bài toán

Chọn một/nhiều contact/phone/email bằng UI hệ thống.

#### API/component nên tái sử dụng

- [Phonebook Fetch UI API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-079-phonebook-fetch-ui-api-md)

#### Quyết định kiến trúc

Dùng Phonebook fetch dialogs thay vì tự viết contact browser.

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

- Libraries: Phonebook UI libs verify
- Capabilities: ReadUserData

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Legacy/deprecated at S60 5th but relevant to FP2.

#### Link chính thức

- [Phonebook Fetch UI API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_Fetch_UI_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-034-vcard-import-export-md"></a>
## vCard import/export

- Original corpus path: `21_REUSE_COOKBOOK/034_vcard_import_export.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-034-vcard-import-export-md`
- `kind`: reuse_recipe

### vCard import/export

#### Bài toán

Trao đổi contacts theo vCard.

#### API/component nên tái sử dụng

- [Phonebook vCard Import Export API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-082-phonebook-vcard-import-export-api-md)

#### Quyết định kiến trúc

Dùng BCard engine/contact model converters.

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
- Capabilities: ReadUserData/WriteUserData

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không tự parse folded lines/charset/version edge cases.

#### Link chính thức

- [Phonebook vCard Import Export API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Phonebook_vCard_Import_Export_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-035-calendar-events-tasks-md"></a>
## Calendar events/tasks

- Original corpus path: `21_REUSE_COOKBOOK/035_calendar_events_tasks.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-035-calendar-events-tasks-md`
- `kind`: reuse_recipe

### Calendar events/tasks

#### Bài toán

Create/read/update calendar entries.

#### API/component nên tái sử dụng

- [Calendar Interim Utils2 API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-016-calendar-interim-utils2-api-md)

#### Quyết định kiến trúc

Dùng Calendar Interim API + Utils2; legacy Calendar API chủ yếu UI definitions.

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

- Libraries: calinterimapi.lib
- Capabilities: ReadUserData, WriteUserData

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Recurrence/time mode/exception semantics rất dễ sai nếu tự model.

#### Guide/class liên quan trong corpus

- [055 ccalsession](43-class-guides-051-075.md#src-12-class-guides-055-ccalsession-md)
- [056 ccalentry](43-class-guides-051-075.md#src-12-class-guides-056-ccalentry-md)

#### Link chính thức

- [Calendar Interim Utils2 API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Calendar_Interim_Utils2_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-036-mms-client-integration-md"></a>
## MMS client integration

- Original corpus path: `21_REUSE_COOKBOOK/036_mms_client_integration.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-036-mms-client-integration-md`
- `kind`: reuse_recipe

### MMS client integration

#### Bài toán

Create/send/manage MMS through MTM.

#### API/component nên tái sử dụng

- [MMS Client MTM API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-065-mms-client-mtm-api-md)

#### Quyết định kiến trúc

Dùng Message Server + MMS Client MTM, không tự implement MMS protocol.

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

- Libraries: msgs.lib + MMS MTM libs verify
- Capabilities: ReadUserData, WriteUserData, NetworkServices as operations require

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

MTM is async/context-driven; account/service entry must exist.

#### Guide/class liên quan trong corpus

- [051 cmsvsession](43-class-guides-051-075.md#src-12-class-guides-051-cmsvsession-md)
- [052 cmsventry](43-class-guides-051-075.md#src-12-class-guides-052-cmsventry-md)

#### Link chính thức

- [MMS Client MTM API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/MMS_Client_MTM_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-037-new-message-indicator-md"></a>
## New message indicator

- Original corpus path: `21_REUSE_COOKBOOK/037_new_message_indicator.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-037-new-message-indicator-md`
- `kind`: reuse_recipe

### New message indicator

#### Bài toán

Thông báo platform component về message mới.

#### API/component nên tái sử dụng

- [New Message Notification API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-073-new-message-notification-api-md)

#### Quyết định kiến trúc

Dùng notification API thay vì tự vẽ global indicator.

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
- Capabilities: Operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Chỉ dùng cho use-case được contract hỗ trợ.

#### Link chính thức

- [New Message Notification API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/New_Message_Notification_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-038-messaging-bio-control-md"></a>
## Messaging BIO control

- Original corpus path: `21_REUSE_COOKBOOK/038_messaging_bio_control.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-038-messaging-bio-control-md`
- `kind`: reuse_recipe

### Messaging BIO control

#### Bài toán

Render/handle smart message type trong messaging UI.

#### API/component nên tái sử dụng

- [Plug-in Bio Control API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-085-plug-in-bio-control-api-md)

#### Quyết định kiến trúc

Implement BIO control contract và registration resources.

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

- Libraries: Messaging BIO libs verify
- Capabilities: ReadUserData as needed

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Plugin lifecycle/UID/resource phải đúng tuyệt đối.

#### Link chính thức

- [Plug-in Bio Control API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Plug-in_Bio_Control_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-039-l-y-v-tr-gps-network-md"></a>
## Lấy vị trí GPS/network

- Original corpus path: `21_REUSE_COOKBOOK/039_l_y_v_tr_gps_network.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-039-l-y-v-tr-gps-network-md`
- `kind`: reuse_recipe

### Lấy vị trí GPS/network

#### Bài toán

Request coordinates, accuracy, satellite info.

#### API/component nên tái sử dụng

- [Location Acquisition API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-064-location-acquisition-api-md)

#### Quyết định kiến trúc

RPositionServer/RPositioner + AO.

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

- Libraries: lbs.lib
- Capabilities: Location

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

SetRequestor, timeout, cancel, no-fix and partial update.

#### Guide/class liên quan trong corpus

- [058 rpositionserver](43-class-guides-051-075.md#src-12-class-guides-058-rpositionserver-md)
- [059 rpositioner](43-class-guides-051-075.md#src-12-class-guides-059-rpositioner-md)

#### Link chính thức

- [Location Acquisition API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Location_Acquisition_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-040-landmark-crud-md"></a>
## Landmark CRUD

- Original corpus path: `21_REUSE_COOKBOOK/040_landmark_crud.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-040-landmark-crud-md`
- `kind`: reuse_recipe

### Landmark CRUD

#### Bài toán

Dùng database địa điểm platform.

#### API/component nên tái sử dụng

- [Landmarks API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-057-landmarks-api-md)
- [Landmarks Database Management API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-058-landmarks-database-management-api-md)

#### Quyết định kiến trúc

CPosLandmarkDatabase và operations.

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

- Libraries: lbslandmarks.lib
- Capabilities: Location/data policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không block operation; DB có thể read-only.

#### Guide/class liên quan trong corpus

- [060 cposlandmarkdatabase](43-class-guides-051-075.md#src-12-class-guides-060-cposlandmarkdatabase-md)

#### Link chính thức

- [Landmarks API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_APIIndexPage.html)
- [Landmarks Database Management API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_Database_Management_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-041-t-m-landmark-g-n-nh-t-md"></a>
## Tìm landmark gần nhất

- Original corpus path: `21_REUSE_COOKBOOK/041_t_m_landmark_g_n_nh_t.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-041-t-m-landmark-g-n-nh-t-md`
- `kind`: reuse_recipe

### Tìm landmark gần nhất

#### Bài toán

Nearest/text/category/area search.

#### API/component nên tái sử dụng

- [Landmarks Search API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-059-landmarks-search-api-md)

#### Quyết định kiến trúc

Criteria + async search/iterator.

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

- Libraries: lbslandmarks.lib
- Capabilities: Location/data policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không load all records rồi distance-sort trong app.

#### Link chính thức

- [Landmarks Search API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_Search_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-042-landmark-selector-editor-md"></a>
## Landmark selector/editor

- Original corpus path: `21_REUSE_COOKBOOK/042_landmark_selector_editor.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-042-landmark-selector-editor-md`
- `kind`: reuse_recipe

### Landmark selector/editor

#### Bài toán

Reuse UI thêm/sửa/chọn landmark/category.

#### API/component nên tái sử dụng

- [Landmarks UI Add Edit API](22-api-catalog-031-060.md#src-10-s60-quick-reference-apis-060-landmarks-ui-add-edit-api-md)
- [Landmarks UI Selector API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-061-landmarks-ui-selector-api-md)

#### Quyết định kiến trúc

Dùng platform dialogs.

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

- Libraries: Landmarks UI libs verify
- Capabilities: Location/data policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Xử lý cancel và DB context.

#### Link chính thức

- [Landmarks UI Add Edit API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_UI_Add_Edit_APIIndexPage.html)
- [Landmarks UI Selector API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Landmarks_UI_Selector_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-043-app-settings-shared-v-i-platform-md"></a>
## App settings shared với platform

- Original corpus path: `21_REUSE_COOKBOOK/043_app_settings_shared_v_i_platform.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-043-app-settings-shared-v-i-platform-md`
- `kind`: reuse_recipe

### App settings shared với platform

#### Bài toán

Typed settings và change notifications.

#### API/component nên tái sử dụng

- [Central Repository Notification Handler API](21-api-catalog-001-030.md#src-10-s60-quick-reference-apis-017-central-repository-notification-handler-api-md)

#### Quyết định kiến trúc

CRepository + notify handler.

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

- Libraries: centralrepository.lib + helper lib
- Capabilities: Key security policy

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Notifications one-shot ở raw API; handler quản lý re-subscribe.

#### Guide/class liên quan trong corpus

- [020 crepository](41-class-guides-001-025.md#src-12-class-guides-020-crepository-md)

#### Link chính thức

- [Central Repository Notification Handler API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Central_Repository_Notification_Handler_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)

---

<a id="src-21-reuse-cookbook-044-profile-read-edit-activate-md"></a>
## Profile read/edit/activate

- Original corpus path: `21_REUSE_COOKBOOK/044_profile_read_edit_activate.md`
- Consolidated reference: `62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-044-profile-read-edit-activate-md`
- `kind`: reuse_recipe

### Profile read/edit/activate

#### Bài toán

Tương tác profile/ringtone/settings hiện có.

#### API/component nên tái sử dụng

- [Profiles Engine Wrapper API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-088-profiles-engine-wrapper-api-md)
- [Profiles Engine Active Profile Settings API](23-api-catalog-061-090.md#src-10-s60-quick-reference-apis-087-profiles-engine-active-profile-settings-api-md)

#### Quyết định kiến trúc

Wrapper/factory thay repository private keys.

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

- Libraries: Profile libs verify
- Capabilities: Operation-specific

Các giá trị “verify” không phải chỗ trống để đoán; dùng class page và local SDK scan.

#### Bẫy chính

Không tự tạo profile nếu wrapper không hỗ trợ.

#### Link chính thức

- [Profiles Engine Wrapper API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Profiles_Engine_Wrapper_APIIndexPage.html)
- [Profiles Engine Active Profile Settings API](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Profiles_Engine_Active_Profile_Settings_APIIndexPage.html)
- [S60 Quick Reference](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/QuickReference.html)
- [C++ API index](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/Cpp_API_ReferenceIndexPage.html)
- [Public file list](https://docs.huihoo.com/symbian/s60-3rd-edition-cpp-developers-library-v1.1/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/files.html)
