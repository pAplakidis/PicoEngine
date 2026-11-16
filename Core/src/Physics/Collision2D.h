#pragma once

#include <glm/glm.hpp>

#include "AABB.h"

namespace PicoEngine
{
  struct CollisionManifold2D
  {
    bool IsColliding = false;
    bool IsTouching = false;

    glm::vec2 Normal{0.0f};
    float Penetration = 0.0f;
  };

  CollisionManifold2D TestAABB(const AABB &a, const AABB &b, float epsilon);
}
