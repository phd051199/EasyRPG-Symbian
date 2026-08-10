# Vendored EasyRPG upstream

This repository vendors the actual upstream EasyRPG source code instead of reimplementing the Player/interpreter.

## EasyRPG Player

- Upstream: EasyRPG/Player
- Version: 0.5.0
- Official source archive: `https://easyrpg.org/downloads/player/0.5.0/easyrpg-player-0.5.0.tar.gz`
- Archive SHA-256: `21c9b95536fb6348e84b11ac8e5908688c6e61e9b4eb426247d546a44869241e`
- Vendored path: `third_party/easyrpg-player-0.5.0/`
- License: GNU GPL v3 or later (retain upstream `COPYING` and per-file notices)

## liblcf

- Upstream: EasyRPG/liblcf
- Version: 0.5.0
- Official source archive: `https://easyrpg.org/downloads/player/0.5.0/liblcf-0.5.0.tar.gz`
- Archive SHA-256: `5f77e40bac5b48ab3163c9628846825e44ae8b5ecb4367de2d55845b2ba48524`
- Vendored path: `third_party/liblcf-0.5.0/`
- License: MIT (retain upstream `COPYING` and per-file notices)

## LLVM C++ standard library runtime

The Symbian ARMv5 build vendors LLVM's official libc++ and libc++abi 4.0.1 source directly in this repository. This release is intentionally from the same era as EasyRPG Player 0.5.0 and provides the C++11 standard-library surface used by Player/liblcf without depending on the host Xcode libc++ ABI.

### libc++

- Upstream: LLVM libc++
- Version: 4.0.1
- Official source archive: `https://releases.llvm.org/4.0.1/libcxx-4.0.1.src.tar.xz`
- Archive SHA-256: `520a1171f272c9ff82f324d5d89accadcec9bc9f3c78de11f5575cdb99accc4c`
- Vendored path: `third_party/libcxx-4.0.1/`
- License: dual University of Illinois/NCSA and MIT; retain `LICENSE.TXT` and per-file notices

### libc++abi

- Upstream: LLVM libc++abi
- Version: 4.0.1
- Official source archive: `https://releases.llvm.org/4.0.1/libcxxabi-4.0.1.src.tar.xz`
- Archive SHA-256: `8f08178989a06c66cd19e771ff9d8ca526dd4a23d1382d63e416c04ea9fa1b33`
- Vendored path: `third_party/libcxxabi-4.0.1/`
- License: dual University of Illinois/NCSA and MIT; retain `LICENSE.TXT` and per-file notices

The ARMv5 lane will use a dependency-driven subset of these upstream trees. Files are copied unchanged from the official release archives; Symbian configuration belongs in build flags or separate platform glue instead of rewriting standard-library implementations.

## Symbian port policy

The vendored source is the implementation of RPG Maker semantics. Do not recreate Player scenes, event commands, map logic, battle logic or liblcf structures in a parallel Symbian-specific engine.

Symbian-specific work belongs in platform adapters whenever possible: filesystem/ZIP VFS, display/pixel presentation, input, timing, audio, font integration and writable save paths.

When an upstream source file must be changed for S60 3rd FP2 / ARMv5 compatibility, keep the change in the vendored file with a small `EASYRPG_SYMBIAN` conditional where practical and document the modified path below. Do not maintain an opaque generated patch stack.

### Upstream files modified for Symbian

None yet. Files currently present in the vendored trees are intended to match the 0.5.0 release byte-for-byte.

## Current direct-vendor inventory

The following Player implementation files are already present directly under `third_party/easyrpg-player-0.5.0/src/` and have been compared byte-for-byte with the official 0.5.0 release during bring-up:

- `player.cpp`, `player.h`
- `game_interpreter.cpp`, `game_interpreter.h`
- `game_map.cpp`, `game_map.h`
- `bitmap.cpp`, `bitmap.h`
- `tilemap.cpp`, `tilemap.h`
- `input.cpp`, `input.h`
- `filefinder.cpp`, `filefinder.h`

The liblcf tree already contains the real upstream reader path and generated RPG model subset needed for LMU bring-up, including:

- `reader_lcf.cpp`, `reader_lcf.h`
- `reader_types.h`, `reader_options.h`
- `reader_util.cpp`, `reader_util.h`
- `lmu_reader.cpp`, `lmu_reader.h`
- `generated/lmu_chunks.h`
- generated `RPG::Map`, `Event`, `EventPage`, `EventCommand`, `MoveRoute` model headers
- upstream `COPYING`

`tools/verify_upstream_vendor.py` compares files already stored in these vendor directories with caller-supplied official release archives. It does not download, generate, patch, or replace the vendored source.

## ARMv5 integration status

The old S60 FP2 GCCE frontend is not the implementation source for EasyRPG C++ code. The Player/liblcf source uses C++11 standard-library facilities that are not provided by the currently installed FP2 SDK headers. `tools/probe_upstream_armv5.sh` probes the actual vendored `reader_lcf.cpp` with an ARMv5 C++11 toolchain.

Do not solve this by translating EasyRPG code to C++03 or by recreating the interpreter. The remaining toolchain job is to provide/link a compatible ARM EABI C++11 standard-library runtime, while keeping the upstream implementation source intact.
