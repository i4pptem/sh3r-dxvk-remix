#pragma once

#include <charconv>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <unordered_map>

namespace dxvk {

// Texture identity belongs to the compatibility layer; the Runtime stores a generic hash-to-curve map.
class WorldUiAlphaCurves {
public:
  bool parse(std::string_view text) {
    m_curves.clear();
    text = trim(text);
    while (!text.empty()) {
      const auto comma = text.find(',');
      const auto entry = trim(text.substr(0, comma));
      const auto separator = entry.find(':');
      if (separator == std::string_view::npos) {
        return reject();
      }
      auto hashText = trim(entry.substr(0, separator));
      const auto valueText = trim(entry.substr(separator + 1));
      if (hashText.substr(0, 2) == "0x" || hashText.substr(0, 2) == "0X") {
        hashText.remove_prefix(2);
      }
      if (hashText.empty() || valueText.empty()) {
        return reject();
      }
      uint64_t hash = 0;
      float exponent = 1.0f;
      const auto hashResult = std::from_chars(hashText.data(), hashText.data() + hashText.size(), hash, 16);
      const auto valueResult = std::from_chars(valueText.data(), valueText.data() + valueText.size(), exponent);
      if (hashResult.ec != std::errc() || hashResult.ptr != hashText.data() + hashText.size() || !hash ||
          valueResult.ec != std::errc() || valueResult.ptr != valueText.data() + valueText.size() ||
          !std::isfinite(exponent) || exponent < 0.25f || exponent > 4.0f ||
          !m_curves.emplace(hash, exponent).second) {
        return reject();
      }
      if (comma == std::string_view::npos) {
        return true;
      }
      text = trim(text.substr(comma + 1));
      if (text.empty()) {
        return reject();
      }
    }
    return true;
  }

  float lookup(uint64_t textureHash) const {
    const auto it = m_curves.find(textureHash);
    return it == m_curves.end() ? 1.0f : it->second;
  }

private:
  static std::string_view trim(std::string_view text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos) {
      return {};
    }
    return text.substr(first, text.find_last_not_of(" \t\r\n") - first + 1);
  }

  bool reject() {
    m_curves.clear();
    return false;
  }

  std::unordered_map<uint64_t, float> m_curves;
};

} // namespace dxvk
