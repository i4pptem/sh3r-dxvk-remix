# SH3 Runtime integration

The compatibility plugin reconstructs game draw state and supplies effect controls. This fork implements rendering semantics that cannot be represented by a replacement texture alone.

## Ownership

- `src/d3d9/d3d9_rtx_ui_policy.h`: classification of native/raster UI, including line primitives.
- `src/d3d9/d3d9_rtx_injection_state.h`: transitions between scene collection, UI and injection.
- `src/dxvk/rtx_render/rtx_context.*`: raster replacement and synchronization.
- `rtx_effect_material.h`: particle emission protocol.
- `rtx_world_ui_alpha_curves.h`: configured alpha curves.
- `src/dxvk/shaders/rtx/concept/surface_material/world_ui_blending.slangh`: World UI blend operations.
- `bridge/src/client/d3d9_surface*`: native surface locking/readback.
- `scripts-common/compile_shaders.py`: shader-compiler invocation support.

The source baseline is NVIDIA commit b474e56b048a0bc132ee77bea7ea664adddf4cbe. All additional source files must accompany the tracked diff; copying only a Git diff loses new headers, shaders and tests.

## Validation and limits

UI-policy and injection-state unit tests are independent of live gameplay. Validate the actual menu, inventory, ESC resume, DOF captures, fades and effect blending in game before publishing a paired release.

Not every overlay reproduces the native compositor pixel-for-pixel. Texture categories, alpha curves, vertex colors and emission settings still affect the result. Changing the private draw-state protocol requires rebuilding and deploying both the compatibility plugin and Runtime.

The release workspace retains the last validated Runtime binary; source packaging does not modify its rendering behavior.
