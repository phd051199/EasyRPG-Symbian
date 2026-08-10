# Audio, video, camera, images, and OpenGL ES

> This file consolidates canonical handbook topics for progressive loading. Each section records its original corpus path and retains external source links.

---

<a id="src-07-multimedia-audio-md"></a>
## Audio Playback and Recording

- Original corpus path: `07_multimedia/audio.md`
- Consolidated reference: `08-multimedia-graphics.md#src-07-multimedia-audio-md`

### Audio Playback and Recording

#### Playback utility

A common high-level API is `CMdaAudioPlayerUtility` with observer callbacks.

Typical dependency:

```cpp
#include <mdaaudiosampleplayer.h>
LIBRARY mediaclientaudio.lib
```

#### Lifecycle

1. Create utility with observer.
2. Open file/descriptor/URL as supported.
3. Wait for initialization callback.
4. Call `Play()`.
5. Receive play-complete callback.
6. Stop/close/delete.

```cpp
void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
void MapcPlayComplete(TInt aError);
```

Do not call `Play()` before successful initialization.

#### Volume

Use `MaxVolume()` and clamp requested volume. Device policy can still limit output.

#### Recording

Recording APIs use separate utilities/observers and may require `UserEnvironment` or other capabilities depending on microphone access policy. Verify exact API.

#### Formats and codecs

The API does not guarantee every codec. Support depends on installed MMF controllers and firmware. Test MP3/AAC/AMR/etc. on the target device.

#### Background and interruptions

Handle phone calls, audio priority conflicts, route changes, and focus loss through completion/errors. Do not assume uninterrupted playback.

---

<a id="src-07-multimedia-camera-md"></a>
## Camera API

- Original corpus path: `07_multimedia/camera.md`
- Consolidated reference: `08-multimedia-graphics.md#src-07-multimedia-camera-md`

### Camera API

#### Main class

The classic camera API centers on `CCamera` and observer interfaces.

Typical dependency:

```cpp
#include <ecam.h>
LIBRARY ecam.lib
```

#### Lifecycle

1. Enumerate/check cameras.
2. Create `CCamera` with observer.
3. Reserve asynchronously.
4. Power on asynchronously.
5. Configure viewfinder/capture.
6. Capture image/video.
7. Release power and reservation.

#### Capability

Camera access commonly requires `UserEnvironment`. Verify device policy and method documentation.

#### Buffers

Observer callbacks can deliver image buffers/bitmaps. Ownership and release rules are strict; read the exact interface contract.

#### Variance

Resolution, format, zoom, flash, autofocus, and viewfinder support differ by device. Query supported values rather than hardcoding.

#### Error handling

`KErrInUse` may indicate another application owns the camera. Always release resources on backgrounding/exit where appropriate.

---

<a id="src-07-multimedia-images-codecs-md"></a>
## Image Decode, Encode, and Bitmap Handling

- Original corpus path: `07_multimedia/images_codecs.md`
- Consolidated reference: `08-multimedia-graphics.md#src-07-multimedia-images-codecs-md`

### Image Decode, Encode, and Bitmap Handling

#### Image Conversion Library

Common classes:

- `CImageDecoder`
- `CImageEncoder`
- `CFrameInfo`
- `CFbsBitmap`

Typical dependency:

```cpp
#include <imageconversion.h>
LIBRARY imageconversion.lib
```

#### Decode flow

1. Create decoder from file/data.
2. Inspect frame info.
3. Create destination bitmap with suitable size/display mode.
4. Call asynchronous `Convert()`.
5. Handle completion in an Active Object.

#### Scaling

Decode to an appropriate size where the API/controller supports it, or scale afterward using bitmap utilities. Full-resolution camera images can exhaust memory.

#### Animated images

Multiple frames require frame iteration and timing. Do not assume a single `Convert()` plays GIF animation automatically.

#### File types

Controller/codec availability is firmware dependent. Detect decoder creation errors and provide fallback.

#### Bitmap server

Some worker threads need an `RFbsSession` connection before bitmap operations. Verify thread setup.

---

<a id="src-07-multimedia-opengl-es-md"></a>
## OpenGL ES 1.x on S60 3rd FP2

- Original corpus path: `07_multimedia/opengl_es.md`
- Consolidated reference: `08-multimedia-graphics.md#src-07-multimedia-opengl-es-md`

### OpenGL ES 1.x on S60 3rd FP2

#### Availability

OpenGL ES support may require the S60 SDK OpenGL ES plug-in and compatible device hardware/firmware.

Common includes/libraries:

```cpp
#include <EGL/egl.h>
#include <GLES/gl.h>
```

Libraries often include `libegl.lib`/`egl.lib` and `libgles_cm.lib`; confirm exact names in the installed plug-in.

#### EGL lifecycle

1. Get display.
2. Initialize EGL.
3. Choose configuration.
4. Create window surface bound to a Symbian window.
5. Create context.
6. Make current.
7. Render and swap buffers.
8. Destroy context/surface and terminate.

#### Constraints

- OpenGL ES 1.x fixed-function pipeline.
- Limited texture memory.
- Context/surface can be affected by focus/background changes.
- Supported extensions vary.

#### UI integration

Create the native window/control first and keep it alive. Coordinate redraw and buffer swap with the application's event loop.

---

<a id="src-07-multimedia-video-md"></a>
## Video Playback with MMF

- Original corpus path: `07_multimedia/video.md`
- Consolidated reference: `08-multimedia-graphics.md#src-07-multimedia-video-md`

### Video Playback with MMF

#### High-level player

`CVideoPlayerUtility` is the common high-level class. Exact constructor and display-rectangle methods differ across platform versions.

Typical dependency:

```cpp
#include <videoplayer.h>
LIBRARY mediaclientvideo.lib
```

Additional graphics/window libraries may be required.

#### Lifecycle

1. Create with observer and window/display parameters expected by FP2.
2. Open media.
3. Wait for open callback.
4. Prepare.
5. Wait for prepare callback.
6. Play.
7. Handle completion/error.

#### Critical compatibility warning

Methods such as display-window or video-rectangle setters vary between S60 3rd FP2, S60 5th, and Symbian^3. If `SetVideoRectL` is undefined, do not declare it manually. Inspect the FP2 `videoplayer.h` and use its available display-window API.

#### Streaming

Network playback support depends on controllers, protocol support, DRM, codecs, and server behavior. A URL accepted by a modern player may not work on FP2.

#### Rendering

The player often needs valid `RWsSession`, `CWsScreenDevice`, window, screen rectangle, and clip rectangle with lifetimes longer than playback.
