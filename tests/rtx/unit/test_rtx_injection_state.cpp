#include <iostream>
#include <stdexcept>

#include "../../../src/d3d9/d3d9_rtx_injection_state.h"

namespace {
  void require(bool condition, const char* message) {
    if (!condition) {
      throw std::runtime_error(message);
    }
  }

  void testPauseResume() {
    dxvk::D3D9RtxInjectionState state;
    for (int frame = 0; frame < 100; ++frame) {
      for (int draw = 0; draw < 8; ++draw) {
        require(!state.tryInjectUi(true), "Pause UI must not consume injection before geometry");
        require(!state.injected(), "Early UI must keep geometry collection open");
      }
      state.reset();
    }
    require(!state.tryInjectUi(true), "Closing-frame background must stay rasterized");
    state.commitGeometry();
    require(!state.injected(), "Resumed geometry must be accepted after pause UI");
    require(state.tryInjectUi(true), "Post-scene compositor must inject on the same frame");
    require(state.injected(), "Injection must latch once a scene exists");
    require(!state.tryInjectUi(true), "Subsequent UI must not inject again");
    state.reset();
    require(!state.tryInjectUi(true), "Previous scene must not qualify the next frame's early UI");
  }

  void testOrdinarySceneAndOptOut() {
    dxvk::D3D9RtxInjectionState state;
    state.commitGeometry();
    require(state.tryInjectUi(true), "Ordinary scene then UI must inject");
    state.commitGeometry();
    require(!state.tryInjectUi(true), "Extra geometry must not reopen an injected frame");
    state.reset();
    state.commitGeometry();
    state.reset();
    require(!state.tryInjectUi(true), "Present-only scene must clear readiness for the next frame");
    require(state.tryInjectUi(false), "Opt-out must allow API-only scenes to retain original behavior");
    require(!state.tryInjectUi(false), "Opt-out still injects at most once");
    state.reset();
    require(state.tryInjectUi(false), "Opt-out must reset at the next frame");
  }
}

int main() {
  try {
    testPauseResume();
    testOrdinarySceneAndOptOut();
    std::cout << "RTX injection state: pause/resume, ordinary scene, frame reset and opt-out passed.\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "RTX injection state test failed: " << error.what() << '\n';
    return -1;
  }
}
