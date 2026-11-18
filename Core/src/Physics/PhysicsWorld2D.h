#pragma once

#include <entt/entt.hpp>

#include "Systems/CollisionSystem2D.h"

namespace PicoEngine
{
  class PhysicsWorld2D
  {
  public:
    void Update(entt::registry &registry, CollisionSystem2D &collisionSystem, float dt);

  private:
    void MoveBody(entt::registry &registry, CollisionSystem2D &collisionSystem, entt::entity entity, float dt);
  };
}
