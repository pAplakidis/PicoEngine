#include "Collision2D.h"

#include <algorithm>
#include <cmath>

namespace PicoEngine
{
  CollisionManifold2D TestAABB(const AABB &a, const AABB &b, float epsilon)
  {
    CollisionManifold2D manifold;

    const glm::vec2 distance = b.GetCenter() - a.GetCenter();
    const glm::vec2 halfSizeSum = (a.GetSize() + b.GetSize()) * 0.5f;
    const float xOverlap = halfSizeSum.x - std::abs(distance.x);
    const float yOverlap = halfSizeSum.y - std::abs(distance.y);

    // Separated beyond contact tolerance
    if (xOverlap < -epsilon || yOverlap < -epsilon)
      return manifold;

    // Both penetrating or close enough to count as touching
    manifold.IsTouching = true;
    manifold.IsColliding = xOverlap > 0.0f && yOverlap > 0.0f;

    // Choose the nearest separation axis
    if (xOverlap < yOverlap)
    {
      manifold.Normal = distance.x < 0.0f ? glm::vec2(-1.0f, 0.0f) : glm::vec2(1.0f, 0.0f);
      manifold.Penetration = std::max(0.0f, xOverlap);
    }
    else
    {
      manifold.Normal = distance.y < 0.0f ? glm::vec2(0.0f, -1.0f) : glm::vec2(0.0f, 1.0f);
      manifold.Penetration = std::max(0.0f, yOverlap);
    }

    return manifold;
  }
}
