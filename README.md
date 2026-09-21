<p align="center"><img src="documentation/sh3r/silent-hill-3-remix.png" alt="SILENT HILL 3 REMIX" width="880"></p>

# sh3r-dxvk-remix

**In the beginning… · v22092026**

The companion RTX Remix Runtime fork for i4pptem's Silent Hill 3 compatibility mod.
This is a game-specific fork of [NVIDIA dxvk-remix](https://github.com/NVIDIAGameWorks/dxvk-remix), not an official NVIDIA release.

## Release baseline

Upstream: `b474e56b048a0bc132ee77bea7ea664adddf4cbe`.
The upstream Git history is preserved. Build artifacts and game binaries do not belong in this source repository.

## SH3 extensions

- World Space UI blend modes, alpha response and per-texture alpha curves.
- Per-particle material emission overrides using the compatibility plugin's draw-state protocol.
- USD albedo/opacity replacement for native raster UI textures.
- Deferred/pre-scene UI injection policy to preserve geometry collection.
- Native snapshot/readback and pause/inventory synchronization.
- Rasterized screen-space line support for inventory elements.
- Unit tests for UI classification and injection-state transitions.

See [technical notes](documentation/sh3r/CHANGES.md) for source ownership and release pairing.

## Build and test

For automatic or manually triggered Windows builds, see [GitHub Actions builds](documentation/sh3r/ACTIONS.md).

Use the [original upstream README](README.upstream.md), the documentation directory and build scripts. Initialize submodules before building. The Runtime is x64; the game-facing bridge is a separate x86 component.

```powershell
git submodule update --init --recursive
.\build_dxvk.ps1 -BuildFlavour release -BuildSubDir _Comp64Release -Backend ninja -EnableTracy false
meson compile -C _Comp64Release
```

Tests use a dedicated build:

```powershell
.\build_dxvk.ps1 -BuildFlavour release -BuildSubDir _Comp64UnitTest -Backend ninja -EnableTracy false -BuildTarget unit_tests -InstallTags unit
meson test -C _Comp64UnitTest test_rasterized_ui_policy test_rtx_injection_state
```

Keep the matching bridge and companion plugin with the Runtime. Do not replace the root bridge DLL with the x64 Runtime DLL.

## Credits and licenses

NVIDIA RTX Remix, DXVK contributors, and the upstream dependencies retain their original licenses and attribution. SH3 compatibility integration: **i4pptem**.
See [LICENSE](LICENSE), [LICENSE-MIT](LICENSE-MIT), [ThirdPartyLicenses.txt](ThirdPartyLicenses.txt) and the [upstream acknowledgements](README.upstream.md).
