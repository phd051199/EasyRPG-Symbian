# Reuse cookbook index

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-21-reuse-cookbook-000-reuse-map-md"></a>
## 000 REUSE MAP

- Original corpus path: `21_REUSE_COOKBOOK/000_REUSE_MAP.md`
- Consolidated reference: `60-reuse-cookbook-index.md#src-21-reuse-cookbook-000-reuse-map-md`

### Reuse map — tra bài toán trước khi tự code lại

| # | Recipe | Bài toán | API/component |
|---:|---|---|---|
| 1 | [Khởi tạo app Avkon đúng chuẩn](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-001-kh-i-t-o-app-avkon-ng-chu-n-md) | Tạo native GUI app có menu, softkey và lifecycle. | Application Framework API, Options Menu API, Status Pane API |
| 2 | [Nhiều màn hình và navigation](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-002-nhi-u-m-n-h-nh-v-navigation-md) | Tách app phức tạp thành nhiều screen có activate/deactivate. | Application Framework API, Navigation Pane API, Tabs API |
| 3 | [Menu động theo trạng thái](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-003-menu-ng-theo-tr-ng-th-i-md) | Dim/add/remove command theo selection, network, playback state. | Options Menu API, Buttons API |
| 4 | [Confirmation, input và message query](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-004-confirmation-input-v-message-query-md) | Hỏi yes/no, nhập text/number/date hoặc chọn list. | Queries API, Dialogs API |
| 5 | [Progress, wait và error note](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-005-progress-wait-v-error-note-md) | Hiển thị tiến trình async và lỗi platform-consistent. | Notes API, Error UI API, Text Resolver API |
| 6 | [Danh sách lớn có search](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-006-danh-s-ch-l-n-c-search-md) | Listbox, model, current item và find pane. | Lists API, Searchfield API, Scroller API |
| 7 | [Màn hình settings](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-007-m-n-h-nh-settings-md) | Edit text, enum, slider, password trong UX chuẩn. | Setting Pages API, Central Repository Notification Handler API |
| 8 | [Skin và icon scalable](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-008-skin-v-icon-scalable-md) | Background, màu, MIF/SVG icon theo skin/layout. | Skins API, Scalable Icons API, Fonts API |
| 9 | [Chọn file/drive chuẩn](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-009-ch-n-file-drive-chu-n-md) | Mở file picker, memory selection, filename prompt. | Common File Dialogs API, Platform Environment API, Directory Localizer API |
| 10 | [Chọn ảnh/audio/video](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-010-ch-n-nh-audio-video-md) | Cho user chọn media theo type. | Media Fetch API |
| 11 | [Đường dẫn hệ thống và disk space](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-011-ng-d-n-h-th-ng-v-disk-space-md) | Lưu cache/data đúng drive/path và tránh disk full. | Platform Environment API, System Utils API |
| 12 | [Feature/version gating](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-012-feature-version-gating-md) | Bật chức năng chỉ khi firmware/hardware hỗ trợ. | Feature Discovery API, Version Info API |
| 13 | [Mở file bằng app hệ thống](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-013-m-file-b-ng-app-h-th-ng-md) | Open/view/edit content có MIME handler sẵn. | Document Handler API |
| 14 | [Send via email/MMS/Bluetooth](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-014-send-via-email-mms-bluetooth-md) | Chia sẻ text/file qua service đã cài. | Send UI API |
| 15 | [Nhúng trang web](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-015-nh-ng-trang-web-md) | Render HTML hoặc web UI trong control. | Browser Control API |
| 16 | [Download có queue/resume/progress](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-016-download-c-queue-resume-progress-md) | Tải file dài, quản lý trạng thái và UI downloads. | Download Manager Engine API, Download Manager UI Library API |
| 17 | [Chọn destination/IAP FP2](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-017-ch-n-destination-iap-fp2-md) | Liệt kê destination/connection method hiện có. | Connection Settings API, Connection Settings UI API |
| 18 | [Theo dõi data connection](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-018-theo-d-i-data-connection-md) | Biết connection tạo/xoá/thay bearer/attributes. | Connection Monitor Server API |
| 19 | [TCP client](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-019-tcp-client-md) | Kết nối server, read/write async. | Connection Settings API |
| 20 | [UDP client/server](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-020-udp-client-server-md) | Datagram send/receive. | Connection Settings API |
| 21 | [HTTP client](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-021-http-client-md) | GET/POST, headers, streaming response. | Connection Settings API |
| 22 | [Bluetooth service/link](61-reuse-cookbook-01-22.md#src-21-reuse-cookbook-022-bluetooth-service-link-md) | Kết nối thiết bị local qua BT. | Feature Discovery API |
| 23 | [Audio playback](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-023-audio-playback-md) | Phát file/URL audio bằng utility platform. | Audio Effects API |
| 24 | [Audio recording](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-024-audio-recording-md) | Record microphone, state callbacks và file output. | Audio Effects API |
| 25 | [Video playback](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-025-video-playback-md) | Open/prepare/play video trong window. | ROP Custom Commands API |
| 26 | [Camera capture](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-026-camera-capture-md) | Reserve camera, viewfinder, still/video capture. | Feature Discovery API |
| 27 | [Decode/encode image](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-027-decode-encode-image-md) | JPEG/PNG/GIF decode, thumbnail và resize. | Images API |
| 28 | [Đọc/sửa EXIF](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-028-c-s-a-exif-md) | Metadata camera, orientation, GPS tags. | Exif API |
| 29 | [EQ/reverb/spatial audio](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-029-eq-reverb-spatial-audio-md) | Điều khiển effect có sẵn trên output/player. | Audio Effects API, Audio Effects Presets API |
| 30 | [Liệt kê thư viện media](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-030-li-t-k-th-vi-n-media-md) | Filter/sort/group media item và metadata. | Content Listing Framework API |
| 31 | [DRM protected content](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-031-drm-protected-content-md) | Check rights, play protected audio, handle expiry. | DRM Helper API, DRM License Checker API, OMA DRM CAF Agent API, DRM Audio Player API |
| 32 | [Contacts CRUD](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-032-contacts-crud-md) | Read/search/edit contact database. | Phonebook Data Management API |
| 33 | [Contact picker](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-033-contact-picker-md) | Chọn một/nhiều contact/phone/email bằng UI hệ thống. | Phonebook Fetch UI API |
| 34 | [vCard import/export](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-034-vcard-import-export-md) | Trao đổi contacts theo vCard. | Phonebook vCard Import Export API |
| 35 | [Calendar events/tasks](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-035-calendar-events-tasks-md) | Create/read/update calendar entries. | Calendar Interim Utils2 API |
| 36 | [MMS client integration](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-036-mms-client-integration-md) | Create/send/manage MMS through MTM. | MMS Client MTM API |
| 37 | [New message indicator](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-037-new-message-indicator-md) | Thông báo platform component về message mới. | New Message Notification API |
| 38 | [Messaging BIO control](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-038-messaging-bio-control-md) | Render/handle smart message type trong messaging UI. | Plug-in Bio Control API |
| 39 | [Lấy vị trí GPS/network](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-039-l-y-v-tr-gps-network-md) | Request coordinates, accuracy, satellite info. | Location Acquisition API |
| 40 | [Landmark CRUD](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-040-landmark-crud-md) | Dùng database địa điểm platform. | Landmarks API, Landmarks Database Management API |
| 41 | [Tìm landmark gần nhất](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-041-t-m-landmark-g-n-nh-t-md) | Nearest/text/category/area search. | Landmarks Search API |
| 42 | [Landmark selector/editor](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-042-landmark-selector-editor-md) | Reuse UI thêm/sửa/chọn landmark/category. | Landmarks UI Add Edit API, Landmarks UI Selector API |
| 43 | [App settings shared với platform](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-043-app-settings-shared-v-i-platform-md) | Typed settings và change notifications. | Central Repository Notification Handler API |
| 44 | [Profile read/edit/activate](62-reuse-cookbook-23-44.md#src-21-reuse-cookbook-044-profile-read-edit-activate-md) | Tương tác profile/ringtone/settings hiện có. | Profiles Engine Wrapper API, Profiles Engine Active Profile Settings API |
| 45 | [Vibration và lights](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-045-vibration-v-lights-md) | Feedback haptic/LED/backlight. | Vibra API, Light API |
| 46 | [Keylock/keyguard](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-046-keylock-keyguard-md) | Query/change keylock qua platform API. | Keylock API |
| 47 | [Auto-start trusted component](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-047-auto-start-trusted-component-md) | Đăng ký startup item khi installer/policy cho phép. | Startup List Management API |
| 48 | [ECom-style plugin](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-048-ecom-style-plugin-md) | Load implementation theo interface UID. | Screen Saver API, Browser Plug-in API |
| 49 | [Invoke service bằng AIW](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-049-invoke-service-b-ng-aiw-md) | Context action/provider discovery. | AIW Criteria API, AIW Generic Parameter API, AIW Service Handler API |
| 50 | [SOAP/web service client](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-050-soap-web-service-client-md) | Service description, credentials, transport properties. | Web Service Connection API, Web Service Description API, Web Service Manager API |
| 51 | [XML model/reader](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-051-xml-model-reader-md) | Build/read XML fragments for Sen/web service. | XML Extensions API |
| 52 | [Localized error message](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-052-localized-error-message-md) | Map error code và show correct note. | Text Resolver API, Error UI API |
| 53 | [Validate phone number](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-053-validate-phone-number-md) | Parse/check phone-number syntax. | Phone Parser API |
| 54 | [Simple notes/text storage](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-054-simple-notes-text-storage-md) | Reuse Notepad data/edit behavior. | Notepad Library API |
| 55 | [OCR](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-055-ocr-md) | Nhận dạng text từ image qua platform OCR server. | Optical Character Recognition API |
| 56 | [Open Flash content](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-056-open-flash-content-md) | Launch platform Flash viewer. | Flash Viewer Framework API |
| 57 | [Screen saver extension](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-057-screen-saver-extension-md) | Implement platform screen-saver plugin. | Screen Saver API |
| 58 | [Instant messaging provider/client](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-058-instant-messaging-provider-client-md) | Integrate IM service/application launch. | Instant Messaging API, IM Application Launch API |
| 59 | [Predictive input/custom dictionary](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-059-predictive-input-custom-dictionary-md) | Interact with PTI engine/input languages. | Predictive Input PTI Engine API, Input Language Info API |
| 60 | [Embedded server application](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-060-embedded-server-application-md) | Expose document/service UI to another app. | Server Application API |
| 61 | [Observe global/window-server events](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-061-observe-global-window-server-events-md) | React to selected WS events outside normal control stack. | Window Server Event Observer API |
| 62 | [Localized folder names](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-062-localized-folder-names-md) | Display human-readable directory names. | Directory Localizer API |
| 63 | [Standard sounds](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-063-standard-sounds-md) | Play key/UI sounds consistent with system. | Sounds API |
| 64 | [Global notification](63-reuse-cookbook-45-64.md#src-21-reuse-cookbook-064-global-notification-md) | Show global note/query/progress outside ordinary dialog context. | Notifiers API |
