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

  struct SweepResult2D
  {
    bool Hit = false;
    float Time = 1.0f; // Normalized fraction of movement [0, 1]
    glm::vec2 Normal{0.0f};
  };

  SweepResult2D SweepAABB(const AABB &moving, const AABB &stationary, const glm::vec2 displacement);
}
