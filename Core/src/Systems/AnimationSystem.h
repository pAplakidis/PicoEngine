#pragma once

#include <entt/entt.hpp>

namespace PicoEngine
{
  class AnimationSystem
  {
  public:
    static void Update(entt::registry &registry, float deltaTime);
  };
}
