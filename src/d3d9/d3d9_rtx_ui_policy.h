#pragma once

#include <cstdint>

namespace dxvk {
  constexpr bool rasterizeUiLines(bool lineTopology, bool positionT,
                                 bool programmableVs, bool depthEnabled) {
    return lineTopology && positionT && !programmableVs && !depthEnabled;
  }

  // Scene geometry and render-to-texture compositors retain their own material paths.
  constexpr uint32_t rasterizedUiTextureMask(bool rasterOnly, bool screenSpace,
                                            uint32_t sampledTextures, uint32_t renderTargets) {
    return rasterOnly && screenSpace ? sampledTextures & ~renderTargets & 0xffffu : 0;
  }
}
