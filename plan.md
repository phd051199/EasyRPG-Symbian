# EasyRPG Player for Nokia E72 — Symbian S60 3rd FP2 Port Plan

## 1. Goal

Build a native EasyRPG Player port for **Nokia E72** (Symbian 9.3 / S60 3rd Edition FP2, ARMv5), with the first compatibility target being the original game archive loaded directly through the runtime ZIP VFS:

- host reference extraction: `.games/NobihazaVH/`
- legacy extracted device path (obsolete; runtime uses the ZIP path documented below): `E:\EasyRPG\Games\NobihazaVH\`

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

## 3. EasyRPG Player build for CSL EABI4

GCCE 3.4.3 cannot parse EasyRPG 0.5-era C++11 source directly. A full mechanical C++03 rewrite would create a large permanent fork.

A working Player build pipeline has now been proven in this repository:

1. host Clang parses/optimizes the EasyRPG Player C++11 source for `arm-none-eabi`, ARMv5T, soft-float;
2. Clang emits ARM assembly only;
3. generated `.eabi_attribute` and `.cpu` metadata directives unsupported by the old CSL assembler are removed; ARM instructions are not rewritten;
4. the S60 SDK `arm-none-symbianelf-as` assembles that code into **EABI4** objects;
5. the S60 SDK `arm-none-symbianelf-ar` builds `EasyRpgPlayer.lib`;
6. the normal GCCE MMP links that static library with the Avkon platform shell;
7. `arm-none-symbianelf-ld` and `elf2e32` generate the final Symbian EXE.

The current `EasyRpgPlayerProbe` C++11 lambda/range-for probe links successfully through this full path. Therefore this is the build route for EasyRPG Player and liblcf.

### ABI boundary rule

Keep the boundary between EasyRPG Player and Symbian platform code narrow and C-compatible:

```text
EasyRPG Player/liblcf C++11
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

EasyRPG Player is compiled with:

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

### DONE — game archive locator + ZIP VFS

Runtime locates `.zip` archives directly. On the target E72, internal storage is preferred and the canonical location is `C:\\Data\\EasyRPG\\Games\\Nobihaza VH.zip`:

1. `C:\Data\EasyRPG\Games\NobihazaVH.zip`
2. `C:\Data\EasyRPG\Games\Nobihaza VH.zip`
3. `C:\Data\NobihazaVH.zip`
4. `C:\Data\Nobihaza VH.zip`
5. optional memory-card fallbacks under `E:\EasyRPG\Games\` and `E:\`

`CZipVfs` opens the archive once, indexes the central directory into hash buckets, resolves the internal project prefix from `RPG_RT.ldb` + `RPG_RT.lmt`, normalizes archive resource names, and reads members directly by local-header offset.

### DONE — native LCF/core bootstrap

The device shell keeps one `RFile` open for the mounted ZIP, gets game bytes through `CZipVfs`, then passes LCF bytes through the C ABI into EasyRPG Player. The Player library now validates/parses enough real LCF structure to bootstrap the target:

- `RPG_RT.ldb`: top-level tagged records + `System.title_name` / `System.system_name` raw CP932 bytes
- `RPG_RT.lmt`: MapInfo array count, tree order, active node, and New Game start map/x/y
- start LMU is selected from LMT as `Map%04u.lmu` instead of hard-coding `Map0001.lmu`
- `Map0001.lmu`: header/top-level record structure and checksum
- stable FNV-1a checksums are returned through the C ABI
- `RPG_RT.ini` game title remains a lightweight diagnostic/display value

For NobihazaVH the regression result is currently `LDB=15`, `LMT=203`, `active_node=53`, `start=M1(1,0)`, `LMU=5`.

This separates Symbian filesystem/path failures from Player parser/interpreter failures and proves that the Player boundary is processing the actual game data.

### DONE — build correctness guard

The build pipeline now deletes the previous target EXE before ABLD. A stale EXE therefore cannot be repackaged after a failed linker invocation.

## 5. NobihazaVH ZIP VFS and filename compatibility

The reference extraction has 21 MiB of data and includes:

- 389 XYZ images
- 202 LMU maps
- 142 WAV files
- 84 PNG images
- 39 MIDI files
- 4 MP3 files

The staging validator currently selects 865 game files / 18,893,078 bytes after excluding the Windows executable, old log, and existing save by default.

### Unicode normalization issue

257 selected paths require NFC normalization. A large number of Japanese filenames are currently decomposed (NFD), while CP932 database strings resolve to precomposed names (NFC). This is a real compatibility problem for asset lookup.

The device runtime mounts the converted ZIP directly. Runtime archives must use ZIP `STORE` for every file and must not be encrypted. Central-directory scanning happens once at startup; resource loads use hash lookup plus positional `RFile::Read` directly against each stored member range. No extraction cache, runtime DEFLATE path, password handling, or ZIP crypto path exists, and ZIP scanning is not part of the per-frame render path.

## 6. EasyRPG core integration plan

### Baseline

Use EasyRPG Player/liblcf **0.5-era semantics** as the compatibility reference for this first game, not current master behavior as the initial porting target.

Do not blindly copy current desktop dependencies into the E72 build.

### Core groups to bring up in order

#### Phase 2A — upstream liblcf 0.5.0 read path

Goal: compile and use the vendored **upstream liblcf 0.5.0 implementation** for LDB/LMT/LMU/LSD. Do not extend the hand-written `CoreLcf.cpp` parser with additional RPG Maker semantics.

Source of truth:

- `third_party/liblcf-0.5.0/` — real upstream release source, pinned in `third_party/UPSTREAM.md`
- Symbian-specific CP932/filesystem adaptation stays at the platform boundary
- `CoreLcf.cpp` is legacy bootstrap/probe code only and must leave the runtime once upstream liblcf is linkable on ARMv5
- no full RTP search yet; report missing resources deterministically

Acceptance / current state:

- [x] parse target LDB without crash
- [x] parse the target LMT MapInfo count/tree order/start data
- [x] resolve New Game start as Map0001 at (1,0) from LMT data
- [x] extract LDB `System.title_name`=`タイトル` and `system_name`=`システム` as CP932
- [x] native Symbian Shift-JIS -> Unicode adapter using `CCnvCharacterSetConverter`
- [x] parse start LMU top-level structure
- [x] report stable checksum/summary back through C ABI
- [x] parse Map defaults/dimensions/chipset id and validate lower+upper layer lengths
- [x] parse event array bootstrap metadata: event/page/declared command counts
- [x] resolve the active map chipset resource name generically from LDB by chipset id
- [x] expose lower/upper tile ids through a bounded C ABI extraction path for runtime ownership
- [ ] parse EventPage fields and serialized EventCommand opcodes/parameters
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

#### Phase 2C — upstream EasyRPG Player renderer

Use the vendored **EasyRPG Player 0.5.0 source** in `third_party/easyrpg-player-0.5.0/`. Do not recreate Tilemap/Bitmap/Scene behavior in a parallel Symbian renderer. Symbian provides display/pixel presentation and any necessary low-level acceleration behind the upstream interfaces.

Bring up in this order:

1. System graphic / basic windows
2. title image/title scene
3. chipset rendering
4. player/character sprites
5. pictures
6. transitions
7. weather/effects/battle graphics as demanded by target

Present remains native 320x240; no scaling pass on E72.

#### Phase 2D — upstream interpreter/runtime

Compile and run the upstream EasyRPG Player 0.5.0 interpreter directly. `game_interpreter.cpp`, `game_map.cpp`, scenes and game-state classes are the behavior source of truth; do not reimplement event commands in Symbian-specific code.

Bring up the required path:

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

### Implemented 2k/2k3 frame-path optimizations

- dirty-rect pipeline keeps up to three separated regions instead of one
  union (`RenderDamage.cpp` `KMaximumDirtyRegions`), with the paint pass
  clipping per region and the presenter blitting per region; the launcher
  display surface coalesces to at most three present regions
- `FrameTimer` discards logic backlog older than four logic frames
  (~66 ms) and re-anchors to the current tick, so a heavy frame can no
  longer suppress rendering while the timer runs unbounded catch-up
- tilemap wall/above sublayers (`z = 98/99`) use the anchored scroll cache
  (memmove shift + edge-strip redraw) exactly like the below-passability
  sublayers, always in A1555 so transparent gaps stay transparent; animated
  upper sublayers keep the direct per-tile path
- animated-tile damage is already gated on animation-step changes, not per
  frame (verified; no extra throttling needed)

### Fresh script-runtime foundation

- the discarded custom script engine, its build targets, tests, bridges, and
  platform classes are removed instead of retained as a historical fallback
- XP, VX, and VX Ace share one isolated `EasyRPGScript.exe` process and one
  pinned Ruby 3.0.7 static library
- the VM is behind a plain C ABI so Ruby's `RArray`/`RFile` types never share a
  translation unit with the same-named Symbian types
- boot selects exactly one of `Scripts.rxdata`, `Scripts.rvdata`, or
  `Scripts.rvdata2`, Marshal-loads the upstream archive, inflates scripts once,
  and evaluates them in source order
- RPG data class definitions come from the small allowlisted mkxp-z 2.4.2
  module files; no desktop/mobile renderer tree is copied into the repository
- packed game archives and native Graphics/Input/Audio bindings are deliberately
  not marked complete; loose script archives are the first bring-up milestone

### Validation status

- Ruby 3.0.7 builds as an 81-object ARMv5 EABI archive and exports the embedding
  and Marshal APIs required by the loader
- `EasyRPGScript.exe` compiles and links successfully for GCCE UREL with the
  new C ABI boundary
- RPG Maker 2000/2003 regression coverage remains separate from the new script
  runtime
- physical E72 boot, memory, Graphics/Input/Audio, packed archive support, and
  gameplay compatibility remain pending

## 10. Build pipeline

### Device build

```sh
bash build-wine.sh build
```

Pipeline:

```text
pinned allowlists + patch files
  -> build/work/third-party

EasyRPG Player C++11
  -> Apple Clang ARMv5 assembly
  -> compatibility metadata filter
  -> CSL arm-none-symbianelf-as (EABI4)
  -> EasyRpgPlayer.lib

Ruby 3.0.7 C99 + E72 ABI shim
  -> GCCE 3.4.3
  -> EasyRpgRuby3.lib

Avkon/platform C++03 + one script runtime
  -> GCCE 3.4.3

all targets
  -> arm-none-symbianelf-ld
  -> elf2e32
  -> makesis
```

Output:

- `sis/EasyRPG.sis` (unsigned UREL SIS)

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
- EasyRPG Player C++11 symbol executes through C ABI (device runtime verification pending)

### M2 — liblcf target parse — PARTIAL, BOOTSTRAP PATH PASSING

- [x] actual Nobihaza LDB/LMT/start-LMU processed by EasyRPG Player
- [x] CP932 title/system resource bytes extracted correctly
- [x] LMT resolves `active_node=53`, New Game `Map0001`, `(1,0)`
- [x] host regression test locks exact target checksums/summary
- [x] Map defaults/dimensions/chipset/layer sizes parsed and validated
- [x] event/page/declared-command counts parsed from nested Event/EventPage arrays
- [x] chipset resource name resolved from the referenced chipset id
- [x] lower/upper tile ids exposed through `EasyRpgCoreExtractMapLayers` with target regression checksums
- [x] chipset terrain/lower-passability/upper-passability exposed through `EasyRpgCoreExtractChipset`
- [ ] EventPage/EventCommand payloads parsed into interpreter-owned structures
- [ ] remaining liblcf 0.5 semantics needed by target

### M3 — first real graphics — PARTIAL, TITLE XYZ PATH IMPLEMENTED

- [x] database-driven `System.title_name` resource lookup (no title wildcard in startup path)
- [x] native Symbian Shift-JIS -> Unicode filename conversion
- [x] XYZ1 + native EZLib/zlib decoder
- [x] palette-indexed XYZ -> clipped RGB565 framebuffer blit
- [x] target title XYZ is released immediately after framebuffer upload
- [ ] physical E72 verification of title display
- [x] exact current ChipSet resource lookup from LDB by map chipset id
- [x] compact target PNG decoder: indexed 8-bit, PLTE/tRNS, IDAT/EZLib, filters 0-4, RGB565 alpha blit
- [x] current start-map chipset PNG decodes and validates as 480x256 during bootstrap, then is released
- [ ] retain/use ChipSet pixels only when entering the real map scene
- [ ] port tilemap block/autotile/transparency/z-order semantics before enabling real map rendering; do not use a naive atlas blit
- [ ] System/Character/Picture resource loaders
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

## 13. One Ruby 3 script runtime for XP, VX, and VX Ace

The script lane is a new implementation under `src/script/`; the removed
project-specific engine is not an alternate build or fallback. Compatibility is
selected from the detected script filename while every game runs in the same
Ruby 3.0.7 VM implementation and the same `EasyRPGScript.exe` binary.

Ownership and lifecycle:

- the launcher detects 2000/2003 versus XP/VX/VX Ace and starts the appropriate
  isolated process
- `CScriptRuntime` owns the ZIP VFS and passes only the script archive bytes and
  generation number across `ScriptVmBridge.h`
- `Ruby3Vm.cpp` owns VM initialization, the rooted Marshal array, one-pass zlib
  inflation, ordered evaluation, error capture, and VM cleanup
- `Ruby3Platform.cpp` owns only E72 libc/thread/atomic/GC-page compatibility
- upstream Ruby and mkxp-z files remain pristine in `third_party/`; all Ruby
  source changes are in `patches/ruby-3.0.7-symbian.patch`

Bring-up sequence:

- [x] one pinned Ruby 3.0.7 source subset and one Ruby static library
- [x] one XP/VX/VX Ace executable and launcher/package dispatch
- [x] clean C ABI between Symbian and Ruby headers
- [x] loose `Scripts.rxdata` / `Scripts.rvdata` / `Scripts.rvdata2` selection
- [x] Ruby Marshal load, one-pass zlib inflate, ordered script evaluation
- [x] upstream RPG data modules selected per game generation
- [x] GCCE ARMv5 UREL compile and link for the new executable
- [ ] packed `Game.rgssad`, `Game.rgss2a`, and `Game.rgss3a` readers
- [ ] native Color/Tone/Rect/Table value types and serialization parity
- [ ] E72 Graphics/Bitmap/Sprite/Viewport/Plane/Tilemap renderer
- [ ] E72 key mapping and generation-specific Input behavior
- [ ] E72 BGM/BGS/ME/SE bridge and resource lookup
- [ ] save paths, fonts, transitions, weather, movie policy, and RTP policy
- [ ] XP/VX/VX Ace compatibility fixtures plus physical-E72 memory/soak tests

Ruby 3 can host both old and newer scripts, but it does not make Ruby 1.8-era
syntax and removed APIs automatically compatible. Compatibility shims must be
small, test-driven, and shared in this one VM; a second Ruby version or a
per-game executable is not permitted.
