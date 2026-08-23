# Pinned third-party source policy

`third_party/` contains only files selected by the manifests in `tools/vendor/`.
Every selected file must be byte-identical to its pinned upstream archive. S60
changes are never edited into this directory; they live in `patches/` and are
applied only to `build/work/third-party/` by `tools/prepare_third_party.sh`.

Run these checks after changing a dependency:

```sh
bash tools/sync_third_party.sh
bash tools/verify_third_party.sh
bash tools/prepare_third_party.sh
```

`sync_third_party.sh` downloads into `build/cache/upstream`, verifies SHA-256,
extracts into `build/import/third-party`, and imports only the explicit
allowlists. Adding a dependency file requires adding it to the corresponding
manifest. Do not copy a complete upstream repository into `third_party/`.

## Pinned components

| Component | Official source | SHA-256 | Allowlist | S60 patch |
|---|---|---|---|---|
| EasyRPG Player 0.5.0 | `https://easyrpg.org/downloads/player/0.5.0/easyrpg-player-0.5.0.tar.gz` | `21c9b95536fb6348e84b11ac8e5908688c6e61e9b4eb426247d546a44869241e` | `tools/vendor/easyrpg-player-0.5.0.files` | `patches/player/easyrpg-player-0.5.0.patch` |
| pixman 0.34.0 | `https://www.cairographics.org/releases/pixman-0.34.0.tar.gz` | `21b6b249b51c6800dc9553b65106e1e37d0e25df942c90531d4c3997aa20a88e` | `tools/vendor/pixman-0.34.0.files` | `patches/platform/pixman-0.34.0.patch` |
| liblcf 0.5.0 | `https://easyrpg.org/downloads/player/0.5.0/liblcf-0.5.0.tar.gz` | `5f77e40bac5b48ab3163c9628846825e44ae8b5ecb4367de2d55845b2ba48524` | `tools/vendor/liblcf-0.5.0.files` | none |
| LLVM libc++ 4.0.1 | `https://releases.llvm.org/4.0.1/libcxx-4.0.1.src.tar.xz` | `520a1171f272c9ff82f324d5d89accadcec9bc9f3c78de11f5575cdb99accc4c` | `tools/vendor/libcxx-4.0.1.files` | `patches/compat/libcxx-4.0.1.patch` |

## Ownership boundary

- Upstream trees own Player/liblcf behavior, pixman, and libc++ sources.
- `patches/series.conf` is the ordered, checksummed patch manifest.
- Patch files own the smallest necessary compiler/OS compatibility deltas.
- Project-owned runtime/platform code lives under `src/`; Symbian build metadata
  lives under `target/`.

Licenses and per-file notices selected by the manifests must remain intact.
