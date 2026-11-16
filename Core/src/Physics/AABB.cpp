#include "AABB.h"

namespace PicoEngine
{

  bool AABB::Intersects(const AABB &other, float epsilon) const
  {
    return min.x <= other.max.x + epsilon &&
           max.x + epsilon >= other.min.x &&
           min.y <= other.max.y + epsilon &&
           max.y + epsilon >= other.min.y;
  }

  glm::vec2 AABB::GetCenter() const
  {
    return (min + max) * 0.5f;
  }

  glm::vec2 AABB::GetSize() const
  {
    return max - min;
  }

  AABB CalculateAABB(const TransformComponent &transform, const BoxCollider2DComponent &boxCollider)
  {
    glm::vec2 scale = glm::vec2(transform.GetScale());
    glm::vec2 position = glm::vec2(transform.GetPosition());

    glm::vec2 halfSize = boxCollider.GetSize() * scale * 0.5f;
    glm::vec2 center = position + boxCollider.GetOffset() * scale;

    return {center - halfSize, center + halfSize};
  }

}
