# EasyRPG Player for Nokia E72

## Scope

This repository is a native Symbian S60 3rd Edition FP2 port of EasyRPG Player
for Nokia E72. The supported game families are RPG Maker 2000 and RPG Maker
2003 only.

The runtime reads game ZIP archives directly and does not emulate `RPG_RT.exe`.
Archives are accepted only when they contain both `RPG_RT.ldb` and
`RPG_RT.lmt` under the same project root.

## Target

- Nokia E72
- Symbian OS 9.3 / S60 3rd Edition FP2
- ARMv5T, soft-float EABI
- 320x240 native game viewport
- S60 3rd FP2 SDK v1.1
- CSL GCCE 3.4.3 for the Symbian platform layer
- host Clang -> ARM assembly -> CSL assembler for the EasyRPG C++11 core

## Repository layout

```text
group/                    Symbian MMP/bld.inf build definitions
inc/                      Symbian-facing headers
patches/                  minimal patches for pinned upstream dependencies
platform/symbian/         Avkon resources and application metadata
sis/                      SIS package definition
src/core/                 portable EasyRPG support code owned by this port
src/platform/symbian/     E72 filesystem/display/input/audio/runtime adapters
tests/                    host regression tests for shared portable helpers
third_party/              pristine allowlisted upstream source files
tools/                    build, vendor-sync and verification scripts
```

There are two device executables:

- `EasyRPG.exe`: Avkon launcher and game archive selector
- `EasyRPGGame.exe`: isolated RPG Maker 2000/2003 player process

No additional emulator or script-runtime process is part of this project.

## Third-party policy

`third_party/` contains only files selected by `tools/vendor/*.files`. Vendored
files stay byte-identical to the pinned upstream archives. Symbian-specific
changes live in `patches/` and are applied to `build/work/third-party/`.

Current dependencies:

- EasyRPG Player 0.5.0
- liblcf 0.5.0
- pixman 0.34.0
- LLVM libc++ 4.0.1

Validation commands:

```sh
bash tools/verify_third_party.sh
bash tools/prepare_third_party.sh
```

## Runtime architecture

The EasyRPG Player/liblcf side is compiled as ARMv5 C++11 and exposed through
a narrow C-compatible boundary. The Symbian side remains GCCE-compatible C++03.
Do not pass STL objects, exceptions, RTTI objects or ABI-sensitive C++ ownership
across that boundary.

The Symbian runtime owns:

- ZIP mounting and resource lookup
- Window Server / direct-screen presentation
- RGB565 display surface
- keyboard state and focus cleanup
- high-resolution frame timing
- native audio and movie adapters
- pause/settings/menu UI
- process lifecycle

The EasyRPG core owns RPG Maker 2000/2003 game semantics.

## Game storage

The launcher scans ZIP archives from:

```text
C:\Data\EasyRPG\Games\
E:\EasyRPG\Games\
```

The ZIP VFS resolves the project root from the directory containing both
`RPG_RT.ldb` and `RPG_RT.lmt`, so archives may contain one enclosing folder.

## Performance rules

- keep the native 320x240 viewport; do not add a final scaling pass
- keep game logic independent from presentation cadence
- do not scan storage every frame
- avoid per-frame heap allocation in rendering and input paths
- use bounded caches and release map-specific assets when no longer needed
- clear held input and suspend ticking on focus loss
- cap timer catch-up after stalls
- prefer dirty-region presentation over unconditional full-screen copies
- profile on physical E72 before speculative micro-optimizations

Implemented frame-path work includes bounded dirty regions, capped timer
backlog, cached tilemap sublayers, and damage gating for animated tiles.

## Build

Device release build and unsigned SIS:

```sh
bash build-wine.sh build
```

Debug build:

```sh
bash build-wine.sh debug
```

Clean generated Symbian outputs:

```sh
bash build-wine.sh clean
```

Build pipeline:

```text
pinned third_party allowlists + patches
  -> build/work/third-party

EasyRPG Player/liblcf C++11
  -> host Clang ARMv5 assembly
  -> CSL arm-none-symbianelf-as
  -> EasyRpgPlayer.lib

Symbian platform C++03
  -> GCCE
  -> EasyRPG.exe + EasyRPGGame.exe
  -> elf2e32
  -> makesis
```

## Acceptance targets

1. Launcher lists only valid RPG Maker 2000/2003 ZIPs.
2. Game process starts from the selected ZIP without extracting it first.
3. RPG Maker 2000 versus 2003 detection comes from LDB metadata.
4. Title, map, event, picture, transition, battle, audio and save behavior stay
   compatible with the pinned EasyRPG 0.5-era baseline required by target games.
5. Input remains state-based and is cleared correctly on focus loss.
6. The player remains stable within practical E72 RAM limits during long play.
7. Release builds produce an unsigned `sis/EasyRPG.sis` containing only the
   launcher, the 2000/2003 runtime and required application resources.

## Current priority

Finish RPG Maker 2000/2003 compatibility and physical E72 validation before
expanding project scope. New engine families should live in separate projects
instead of adding parallel runtimes and third-party stacks here.

## Licensing

Retain upstream licenses and notices for all imported EasyRPG Player, liblcf,
pixman and libc++ files. Keep upstream-derived code clearly separated from the
original Symbian platform adapters and document pinned revisions in
`third_party/UPSTREAM.md`.
