#pragma once

#include <entt/entt.hpp>

#include "Collision2D.h"

namespace PicoEngine
{
  struct CollisionContact2D
  {
    entt::entity EntityA = entt::null;
    entt::entity EntityB = entt::null;

    CollisionManifold2D Manifold;

    bool IsTrigger = false;
  };
}
