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

## LLVM C++ standard library headers

The Symbian ARMv5 build vendors the dependency-driven libc++ 4.0.1 header subset used by Player/liblcf without depending on the host Xcode libc++ ABI.


- Upstream: LLVM libc++
- Version: 4.0.1
- Official source archive: `https://releases.llvm.org/4.0.1/libcxx-4.0.1.src.tar.xz`
- Archive SHA-256: `520a1171f272c9ff82f324d5d89accadcec9bc9f3c78de11f5575cdb99accc4c`
- Vendored path: `third_party/libcxx-4.0.1/`
- License: dual University of Illinois/NCSA and MIT; retain `LICENSE.TXT` and per-file notices

The ARMv5 lane uses only the referenced libc++ headers. Symbian configuration and runtime compatibility live in the platform glue instead of a vendored libc++abi tree.

## Ruby MRI

The RGSS runtime vendors a curated Ruby MRI core subset rather than keeping an extracted upstream tarball in the build tree.

- Upstream: Ruby MRI
- Version: 3.0.7
- Official source archive: `https://cache.ruby-lang.org/pub/ruby/3.0/ruby-3.0.7.tar.gz`
- Archive SHA-256: `2a3411977f2850431136b0fab8ad53af09fb74df2ee2f4fb7f11b378fe034388`
- Vendored path: `third_party/ruby-3.0.7/`
- License: Ruby License / BSD-2-Clause components; retain upstream `COPYING` and `LEGAL`
- Scope: only MRI core translation units compiled by `tools/build_rgss_mri.sh`, directly included core fragments, required root headers/generated includes, public/internal Ruby headers, ARM coroutine support, selected `missing/` compatibility code, and Unicode data used by the compiled encoders
- Explicitly excluded: `ext/`, `test/`, `spec/`, `doc/`, `benchmark/`, `sample/`, `gems/`, `lib/`, `tool/`, platform source trees not used by the Symbian ARM build, and other release-package material unrelated to the embedded VM

`tools/fetch_rgss_mri.sh` is a maintenance importer for refreshing this curated dependency. Normal builds never fetch Ruby and compile only the checked-in `third_party/ruby-3.0.7` tree.

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
- `reader_util.h`
- `lmu_reader.cpp`, `lmu_reader.h`
- `generated/lmu_chunks.h`
- generated `RPG::Map`, `Event`, `EventPage`, `EventCommand`, `MoveRoute` model headers
- upstream `COPYING`

## ARMv5 integration status

The old S60 FP2 GCCE frontend is not the implementation source for EasyRPG C++ code. Player/liblcf are compiled through the modern ARMv5 core pipeline, with Symbian-specific compatibility supplied by the platform adapters and compatibility runtime in this repository.
