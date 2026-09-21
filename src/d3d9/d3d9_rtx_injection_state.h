#pragma once

namespace dxvk {
  // NV-DXVK start: UI injection lifetime
  class D3D9RtxInjectionState {
  public:
    bool injected() const {
      return m_injected;
    }

    bool tryInjectUi(bool deferUntilGeometry) {
      if (m_injected || (deferUntilGeometry && !m_hasGeometry)) {
        return false;
      }
      m_injected = true;
      return true;
    }

    void commitGeometry() {
      m_hasGeometry = true;
    }

    void reset() {
      m_injected = false;
      m_hasGeometry = false;
    }

  private:
    bool m_injected = false;
    bool m_hasGeometry = false;
  };
  // NV-DXVK end
}
