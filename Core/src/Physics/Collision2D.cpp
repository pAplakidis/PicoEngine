#include "Collision2D.h"

#include <algorithm>
#include <cmath>
#include <limits>

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

  SweepResult2D SweepAABB(const AABB &moving, const AABB &stationary, const glm::vec2 displacement)
  {
    SweepResult2D result;

    if (displacement.x == 0.0f && displacement.y == 0.0f)
      return result;

    // initial overlaps require separate penetration recovery.
    if (moving.min.x < stationary.max.x &&
        moving.max.x > stationary.min.x &&
        moving.min.y < stationary.max.y &&
        moving.max.y > stationary.min.y)
      return result;

    const float infinity = std::numeric_limits<float>::infinity();

    float xEntry = -infinity;
    float xExit = infinity;
    float yEntry = -infinity;
    float yExit = infinity;

    // x-axis
    if (displacement.x > 0.0f)
    {
      xEntry = (stationary.min.x - moving.max.x) / displacement.x;
      xExit = (stationary.max.x - moving.min.x) / displacement.x;
    }
    else if (displacement.x < 0.0f)
    {
      xEntry = (stationary.max.x - moving.min.x) / displacement.x;
      xExit = (stationary.min.x - moving.max.x) / displacement.x;
    }
    else
    {
      // no x movement, the intervals must already overlap
      if (moving.max.x <= stationary.min.x || moving.min.x >= stationary.max.x)
        return result;
    }

    // y-axis
    if (displacement.y > 0.0f)
    {
      yEntry = (stationary.min.y - moving.max.y) / displacement.y;
      yExit = (stationary.max.y - moving.min.y) / displacement.y;
    }
    else if (displacement.y < 0.0f)
    {
      yEntry = (stationary.max.y - moving.min.y) / displacement.y;
      yExit = (stationary.min.y - moving.max.y) / displacement.y;
    }
    else
    {
      // no y movement, the intervals must already overlap
      if (moving.max.y <= stationary.min.y ||
          moving.min.y >= stationary.max.y)
        return result;
    }

    const float entryTime = std::max(xEntry, yEntry);
    const float exitTime = std::min(xExit, yExit);

    // no collision withing this displacement
    if (entryTime > exitTime || entryTime < 0.0f || entryTime > 1.0f || exitTime < 0.0f)
      return result;

    result.Hit = true;
    result.Time = entryTime;

    // Normal points away from the obstacle, opposite to movement along the impact axis
    if (xEntry > yEntry)
    {
      result.Normal = {displacement.x > 0.0f ? -1.0f : 1.0f, 0.0f};
    }
    else
    {
      result.Normal = {0.0f, displacement.y > 0.0f ? -1.0f : 1.0f};
    }

    return result;
  }
}
