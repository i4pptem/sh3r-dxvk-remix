#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>

namespace dxvk {
// Per-draw compatibility protocol. Unused D3D9 states are transported by the bridge.
constexpr uint32_t kEffectMaterialMarkerState = 149;
constexpr uint32_t kEffectMaterialEmissionState = 169;
constexpr uint32_t kEffectMaterialMarker = 0x53485031; // SHP1

inline float decodeEffectEmission(uint32_t marker, uint32_t bits) {
  float value;
  std::memcpy(&value, &bits, sizeof(value));
  return marker == kEffectMaterialMarker && std::isfinite(value) && value >= 0.0f && value <= 4096.0f
    ? value : -1.0f;
}
}
