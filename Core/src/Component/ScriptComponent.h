#pragma once

#include <functional>

namespace PicoEngine
{

  class Entity;

  class ScriptComponent
  {
  public:
    using SetupFunction = std::function<void(Entity)>;
    using LoopFunction = std::function<void(Entity, float)>;

    ScriptComponent() = default;

    ScriptComponent(SetupFunction setup, LoopFunction loop)
        : Setup(std::move(setup)),
          Loop(std::move(loop))
    {
    }

    bool IsInitialized() { return m_Initialized; }
    void Initialize() { m_Initialized = true; }

    SetupFunction Setup;
    LoopFunction Loop;

  private:
    bool m_Initialized = false;
  };
}
