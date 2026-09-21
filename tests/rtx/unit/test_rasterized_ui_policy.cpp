#include "../../../src/d3d9/d3d9_rtx_ui_policy.h"
#include "../../../src/d3d9/d3d9_rtx_injection_state.h"

#include <iostream>

int main() {
  using dxvk::rasterizedUiTextureMask;
  static_assert(rasterizedUiTextureMask(true, true, 1, 0) == 1, "Native UI stage 0");
  static_assert(rasterizedUiTextureMask(true, true, 0x8005, 0) == 0x8005, "Multiple sampled UI stages");
  static_assert(rasterizedUiTextureMask(false, true, 1, 0) == 0, "RT submission must never receive a raster replacement");
  static_assert(rasterizedUiTextureMask(true, false, 1, 0) == 0, "Preserved world geometry is not UI");
  static_assert(rasterizedUiTextureMask(true, true, 0, 0) == 0, "Untextured UI");
  static_assert(rasterizedUiTextureMask(true, true, 0xffff, 0xffff) == 0, "Compositor render targets");
  static_assert(rasterizedUiTextureMask(true, true, 7, 2) == 5, "Mixed UI and render-target samplers");
  static_assert(rasterizedUiTextureMask(true, true, 0xffff0001, 0) == 1, "Only pixel samplers");
  using dxvk::rasterizeUiLines;
  static_assert(rasterizeUiLines(true, true, false, false), "Inventory RHW lines must rasterize");
  static_assert(!rasterizeUiLines(false, true, false, false), "Fog, glows, triangles and points keep their routes");
  static_assert(!rasterizeUiLines(true, false, false, false), "World-space lines are not screen UI");
  static_assert(!rasterizeUiLines(true, true, true, false), "A programmable VS may override RHW positions");
  static_assert(!rasterizeUiLines(true, true, false, true), "Depth-tested lines keep their route");
  for (unsigned flags = 0; flags < 16; ++flags) {
    if (rasterizeUiLines(flags & 1, flags & 2, flags & 4, flags & 8) != (flags == 3)) {
      std::cerr << "Screen-space line classification failed for flags " << flags << '\n';
      return -1;
    }
  }
  dxvk::D3D9RtxInjectionState injection;
  if (injection.tryInjectUi(true) || injection.injected()) {
    std::cerr << "Inventory lines must not close geometry collection in a UI-only frame.\n";
    return -1;
  }
  injection.commitGeometry();
  if (!injection.tryInjectUi(true) || injection.tryInjectUi(true)) {
    std::cerr << "UI lines after scene geometry must inject RTX exactly once.\n";
    return -1;
  }
  std::cout << "Rasterized UI policy: sampler masks, 16 line classifications and UI injection order passed.\n";
  return 0;
}
