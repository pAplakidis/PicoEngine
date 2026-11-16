#pragma once

#include <glm/glm.hpp>

#include "Component/TransformComponent.h"
#include "Component/BoxCollider2DComponent.h"

namespace PicoEngine
{
  struct AABB
  {
    glm::vec2 min;
    glm::vec2 max;

    bool Intersects(const AABB &other, float epsilon = 0.0f) const;
    glm::vec2 GetCenter() const;
    glm::vec2 GetSize() const;
  };

  AABB CalculateAABB(const TransformComponent &transform, const BoxCollider2DComponent &boxCollider);

}
