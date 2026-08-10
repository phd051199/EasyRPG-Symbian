# EasyRPG Player for Nokia E72 — Symbian S60 3rd FP2 Port Plan

## 1. Goal

Build a native EasyRPG Player port for **Nokia E72** (Symbian 9.3 / S60 3rd Edition FP2, ARMv5), with the first compatibility target being the extracted game in:

- host test data: `.games/NobihazaVH/`
- device canonical path: `E:\EasyRPG\Games\NobihazaVH\`

The first target game is confirmed locally as an RPG Maker 2000 project:

- `RPG_RT.ldb` magic: `LcfDataBase`
- `RPG_RT.lmt` magic: `LcfMapTree`
- `Map0001.lmu` magic: `LcfMapUnit`
- title: `Doraemon: Nobita's Resident Evil`
- old bundled EasyRPG log used the RPG2k interpreter and detected CP932/IBM-943 encoding
- the old log identifies an EasyRPG 0.5.0-era Player, so **0.5-era behavior is the initial semantic compatibility baseline**

This project is a native Player/runtime port. It does not emulate Windows `RPG_RT.exe`.

## 2. Hard target constraints

### Platform

- Nokia E72
- Symbian OS 9.3
- S60 3rd Edition FP2
- ARMv5T / soft-float EABI
- device display: 320x240
- SDK: `S60_3rd_FP2_SDK_v1.1`
- device linker/toolchain: CSL GCCE 3.4.3 + `elf2e32`

### Runtime

- native internal game viewport: **320x240**
- no final framebuffer scaling on E72
- logic cadence: **60 Hz**
- presentation target: **30 Hz** initially
- logic rate must not be coupled to rendering rate
- pause rendering/game ticks when app loses foreground
- release all pressed-key state on focus/foreground loss

### Memory

The E72 has very limited practical free RAM. The port must be streaming/cache-based and must not preload the game.

Initial working budgets (to be verified on a physical E72):

- executable + static data: keep as small as practical
- framebuffer(s): < 1 MiB
- decoded bitmap cache: target <= 8-10 MiB
- audio decode/mix buffers: target <= 2-4 MiB
- map/event/runtime state: bounded and released on transition
- total process working set target: <= 30-35 MiB under ordinary gameplay
- MMP hard heap ceiling currently: 40 MiB; this is a ceiling, not a target

## 3. Key architecture decision: hybrid C++11 -> CSL EABI4

GCCE 3.4.3 cannot parse EasyRPG 0.5-era C++11 source directly. A full mechanical C++03 rewrite would create a large permanent fork.

A working hybrid pipeline has now been proven in this repository:

1. modern Clang parses/optimizes the C++11 core source for `arm-none-eabi`, ARMv5T, soft-float;
2. Clang emits ARM assembly only;
3. generated `.eabi_attribute` and `.cpu` metadata directives unsupported by the old CSL assembler are removed; ARM instructions are not rewritten;
4. the S60 SDK `arm-none-symbianelf-as` assembles that code into **EABI4** objects;
5. the S60 SDK `arm-none-symbianelf-ar` builds `EasyRpgModernCore.lib`;
6. the normal GCCE MMP links that static library with the Avkon platform shell;
7. `arm-none-symbianelf-ld` and `elf2e32` generate the final Symbian EXE.

The current `EasyRpgModernCoreProbe` C++11 lambda/range-for probe links successfully through this full path. Therefore this is the primary route for the EasyRPG/liblcf gameplay core.

### ABI boundary rule

Keep the boundary between modern core and Symbian platform code narrow and C-compatible:

```text
EasyRPG/liblcf-derived C++11 core
        |
        | extern "C" bridge / POD buffers / integer handles
        v
Symbian platform adapter (GCCE C++03)
        |
        +-- RFs/RFile
        +-- CFbsBitmap / Window Server
        +-- RTimer / Active Objects
        +-- key events
        +-- native audio/MMF
```

Do not pass STL objects, exceptions, RTTI objects, C++ ownership, or C++ ABI-sensitive types across this boundary.

The modern core is compiled initially with:

- `-fno-exceptions`
- `-fno-rtti`
- `-fno-threadsafe-statics`
- `-ffreestanding`
- ARMv5T soft-float

Any upstream subsystem that requires exceptions/unsupported runtime library behavior must be adapted rather than leaking it across the bridge.

## 4. Current implemented platform layer

### DONE — native Avkon application shell

- `E32Main -> CAknApplication -> CAknDocument -> CAknAppUi`
- UID3: `0xE31EA5A1` (development UID for this port)
- `CAPABILITY None`
- fullscreen application
- status pane hidden
- CBA hidden
- native S60 resource + registration resources
- GCCE UREL build
- unsigned SIS packaging

### DONE — framebuffer

- `CFbsBitmap`
- `EColor64K` / RGB565
- fixed 320x240 surface
- direct `LockHeap()` / `DataAddress()` access
- Window Server `BitBlt` presentation

The Player core should render to an abstract pixel surface. The Symbian present adapter should own final conversion to RGB565. Avoid making RGB565 assumptions inside gameplay logic.

### DONE — timing

- `RTimer::HighRes`
- 16,667 us logic tick (60 Hz nominal)
- present every second logic tick (30 Hz)
- next timer request is armed before frame work to reduce drift

Future work: add accumulator/catch-up rules with a strict cap so a slow frame cannot create an unbounded event-loop spiral.

### DONE — input foundation

Mapped key-state input (keydown/keyup, not one-shot only):

- arrows -> movement
- D-pad center / Enter / Z / Space -> decision
- Escape / Backspace / X -> cancel
- Shift / A -> shift
- `1` -> debug placeholder

Pressed states are cleared on focus/foreground loss.

### DONE — foreground lifecycle

- stop game timer when backgrounded
- clear pressed keys
- restart timer only when foregrounded

### DONE — game locator

Preferred roots:

1. `E:\EasyRPG\Games\NobihazaVH\`
2. `E:\EasyRPG\Games\Nobihaza VH\`
3. `E:\Nobihaza VH\`
4. `C:\Data\EasyRPG\Games\NobihazaVH\`
5. `C:\Data\EasyRPG\Games\Nobihaza VH\`

A root is accepted only when both `RPG_RT.ldb` and `RPG_RT.lmt` exist.

### DONE — native LCF/core bootstrap

The device shell uses `RFs/RFile` only for I/O, then passes raw bytes through the C ABI into the modern C++11 core. The current core now validates/parses enough real LCF structure to bootstrap the target:

- `RPG_RT.ldb`: top-level tagged records + `System.title_name` / `System.system_name` raw CP932 bytes
- `RPG_RT.lmt`: MapInfo array count, tree order, active node, and New Game start map/x/y
- start LMU is selected from LMT as `Map%04u.lmu` instead of hard-coding `Map0001.lmu`
- `Map0001.lmu`: header/top-level record structure and checksum
- stable FNV-1a checksums are returned through the C ABI
- `RPG_RT.ini` game title remains a lightweight diagnostic/display value

For NobihazaVH the regression result is currently `LDB=15`, `LMT=203`, `active_node=53`, `start=M1(1,0)`, `LMU=5`.

This separates Symbian filesystem/path failures from core parser/interpreter failures and proves that the modern-core boundary is processing the actual game data.

### DONE — build correctness guard

The build pipeline now deletes the previous target EXE before ABLD. A stale EXE therefore cannot be repackaged after a failed linker invocation.

## 5. NobihazaVH staging and filename compatibility

The extracted target has 21 MiB of data and includes:

- 389 XYZ images
- 202 LMU maps
- 142 WAV files
- 84 PNG images
- 39 MIDI files
- 4 MP3 files

The staging validator currently selects 865 game files / 18,893,078 bytes after excluding the Windows executable, old log, and existing save by default.

### Unicode normalization issue

257 selected paths require NFC normalization. A large number of Japanese filenames are currently decomposed (NFD), while CP932 database strings resolve to precomposed names (NFC). This is a real compatibility problem for asset lookup.

`tools/prepare_nobihaza.py` therefore:

- validates the RPG2k project markers;
- normalizes every staged path component to NFC;
- case-folds destination keys and rejects collisions before copying;
- skips `RPG_RT.exe` and old logs;
- skips existing `Save*.lsd` by default for clean-device testing;
- leaves `.games/NobihazaVH` untouched.

The device runtime does **not** need to decrypt/extract the distribution ZIP in phase 1. It consumes an extracted, normalized project directory. This keeps ZIP/AES/Unicode archive handling out of the E72 hot path and lowers memory/code size.

## 6. EasyRPG core integration plan

### Baseline

Use EasyRPG Player/liblcf **0.5-era semantics** as the compatibility reference for this first game, not current master behavior as the initial porting target.

Do not blindly copy current desktop dependencies into the E72 build.

### Core groups to bring up in order

#### Phase 2A — liblcf read path

Goal: parse the target's real database/map tree/Map0001 through the modern-core static library.

Implement/adapt:

- LCF binary reader
- LDB
- LMT
- LMU
- required CP932 -> Unicode conversion adapter
- no full RTP search yet; report missing resources deterministically

Acceptance / current state:

- [x] parse target LDB without crash
- [x] parse the target LMT MapInfo count/tree order/start data
- [x] resolve New Game start as Map0001 at (1,0) from LMT data
- [x] extract LDB `System.title_name`=`タイトル` and `system_name`=`システム` as CP932
- [x] native Symbian Shift-JIS -> Unicode adapter using `CCnvCharacterSetConverter`
- [x] parse start LMU top-level structure
- [x] report stable checksum/summary back through C ABI
- [ ] parse Map dimensions/chipset/lower+upper layers/event array into runtime structures
- [ ] identify all 0.5-era liblcf behaviors required by the target beyond the bootstrap subset

#### Phase 2B — image loading

Priority for this game:

1. XYZ decoder (389 files; highest priority)
2. PNG decoder (84 files)
3. cache/lookup layer with NFC/case-insensitive semantics matching staged names

Avoid loading whole image directories.

Initial cache policy:

- retain current map/system/player assets
- LRU for pictures/characters
- explicit purge at map transition / memory pressure
- no duplicate decoded copy after RGB565 upload if not required by alpha/effects

#### Phase 2C — Player renderer

Bring up in this order:

1. System graphic / basic windows
2. title image/title scene
3. chipset rendering
4. player/character sprites
5. pictures
6. transitions
7. weather/effects/battle graphics as demanded by target

Present remains native 320x240; no scaling pass on E72.

#### Phase 2D — interpreter/runtime

Bring across the RPG2k interpreter required by the target:

- game state
- switches / variables
- event pages
- move routes
- message/window commands
- map transfer
- pictures
- common events / parallel events
- save/load state
- battle only after normal-map/event path is stable

Use the 0.5-era target behavior as the first regression reference.

#### Phase 2E — font/text

The target is Vietnamese + Japanese-origin asset names/data.

Requirements:

- preserve CP932 decoding for original RPG data
- render Vietnamese text correctly
- avoid full HarfBuzz/ICU dependency on E72
- prefer a compact glyph/font path; system font fallback can be used while bringing up logic
- cache rasterized glyphs with a bounded cache

The old game log contains missing-glyph warnings, so text rendering must be tested explicitly rather than assumed correct.

#### Phase 2F — audio

Add only what the target needs, in this order:

1. WAV/SFX
2. MP3 music
3. MIDI

Preferred E72 design:

- native Symbian MMF/streaming where possible
- small PCM ring buffers
- no FluidSynth/SoundFont
- do not decode entire MP3/WAV tracks into RAM

MIDI should prefer a native Symbian MIDI path if it provides behavior acceptable for the target; software SoundFont synthesis is not suitable for the E72 memory budget.

## 7. RTP strategy

The old EasyRPG run added an RPG Maker 2000 RTP path and reported some missing sounds. Do not bundle proprietary RTP blindly.

Process:

1. instrument all missing-resource lookups;
2. determine which resources are truly absent from the game directory;
3. distinguish filename-normalization misses from real RTP dependencies;
4. only then decide the legal/user-supplied RTP installation path.

## 8. Save strategy

Do not modify the original game directory during host tests.

On device, save data should eventually go to a dedicated writable path rather than depending on install drive placement, e.g. an EasyRPG data directory under the selected writable drive.

Initial clean test deliberately excludes the bundled `Save15.lsd`.

## 9. Performance rules

- never scan the entire memory card per frame/startup
- no per-frame filename Unicode normalization
- no per-frame heap allocation in renderer/input
- use fixed/scratch buffers where practical
- bounded image/audio caches
- release map-specific assets immediately after transition when no longer referenced
- 60 Hz game logic remains independent from 30 Hz presentation
- cap catch-up work after stalls
- no background rendering/ticking
- prefer integer/fixed-point math in hot paths where upstream floating point is unnecessary
- profile on physical E72 before speculative micro-optimization

## 10. Build pipeline

### Game validation

```sh
python3 tools/prepare_nobihaza.py --check
```

### Stage to a mounted memory card

```sh
python3 tools/prepare_nobihaza.py \
  --dest /Volumes/<E72_CARD>/EasyRPG/Games/NobihazaVH
```

### Device build

```sh
bash build-wine.sh build
```

Pipeline:

```text
modern core C++11
  -> Apple Clang ARMv5 assembly
  -> compatibility metadata filter
  -> CSL arm-none-symbianelf-as (EABI4)
  -> EasyRpgModernCore.lib

Avkon platform C++03
  -> GCCE 3.4.3

both
  -> arm-none-symbianelf-ld
  -> elf2e32
  -> makesis
```

Output:

- `sis/EasyRPG_S60_3_X_v_0_1_0.sis` (unsigned development SIS)

## 11. Milestones / acceptance tests

### M0 — build shell — DONE

- GCCE UREL compiles
- resources compile
- `elf2e32` succeeds
- unsigned SIS generated

### M1 — E72 platform proof — IMPLEMENTED, PHYSICAL DEVICE TEST PENDING

- fullscreen 320x240
- RGB565 framebuffer
- 60 Hz logic / 30 Hz present
- keydown/keyup state
- focus cleanup
- locate game directory
- native LCF magic probe
- C++11 modern-core symbol executes through C ABI (device runtime verification pending)

### M2 — liblcf target parse — PARTIAL, BOOTSTRAP PATH PASSING

- [x] actual Nobihaza LDB/LMT/start-LMU processed in modern core
- [x] CP932 title/system resource bytes extracted correctly
- [x] LMT resolves `active_node=53`, New Game `Map0001`, `(1,0)`
- [x] host regression test locks exact target checksums/summary
- [ ] Map fields/layers/events parsed into runtime-owned structures
- [ ] remaining liblcf 0.5 semantics needed by target

### M3 — first real graphics — PARTIAL, TITLE XYZ PATH IMPLEMENTED

- [x] database-driven `System.title_name` resource lookup (no title wildcard in startup path)
- [x] native Symbian Shift-JIS -> Unicode filename conversion
- [x] XYZ1 + native EZLib/zlib decoder
- [x] palette-indexed XYZ -> clipped RGB565 framebuffer blit
- [x] target title XYZ is released immediately after framebuffer upload
- [ ] physical E72 verification of title display
- [ ] System/ChipSet/Character/Picture resource loaders
- [ ] PNG decoder path
- [ ] remove diagnostic renderer after the real title/map scenes own the full frame

### M4 — title scene

- title menu rendered by EasyRPG-derived Player code
- input works

### M5 — first playable map

- new game
- Map0001 loads
- player walks
- messages/events/transfers work

### M6 — audio + save

- required WAV/MP3/MIDI paths
- save/load survives restart

### M7 — target-game compatibility pass

- play through NobihazaVH
- fix event/transition/picture/battle edge cases
- memory leak soak test
- stable 30 FPS presentation where workload permits

## 12. Licensing

EasyRPG Player/liblcf code brought into this repository must retain the upstream license/notices and satisfy the applicable GPL source-distribution requirements. Keep upstream-derived code clearly separated from original Symbian platform adapters and document the exact upstream revision used.
