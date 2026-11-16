#include "CollisionSystem2D.h"

#include "Component/TransformComponent.h"
#include "Component/BoxCollider2DComponent.h"

#include "Physics/AABB.h"
#include "Physics/Collision2D.h"

#include "Util/Log.h"

namespace PicoEngine
{
  void CollisionSystem2D::Update(entt::registry &registry)
  {
    m_Contacts.clear();

    auto view = registry.view<TransformComponent, BoxCollider2DComponent>();

    for (auto itA = view.begin(); itA != view.end(); ++itA)
    {
      const auto entityA = *itA;
      const auto &transformA = view.get<TransformComponent>(entityA);
      const auto &colliderA = view.get<BoxCollider2DComponent>(entityA);
      const AABB aabbA = CalculateAABB(transformA, colliderA);

      auto itB = itA;
      ++itB;
      for (; itB != view.end(); ++itB)
      {
        const auto entityB = *itB;
        const auto &transformB = view.get<TransformComponent>(entityB);
        const auto &colliderB = view.get<BoxCollider2DComponent>(entityB);
        const AABB aabbB = CalculateAABB(transformB, colliderB);
        const CollisionManifold2D manifold = TestAABB(aabbA, aabbB, 0.0001f);

        LOG_TRACE(
            "A min=({}, {}) max=({}, {}) | "
            "B min=({}, {}) max=({}, {}) | "
            "Touching={} Colliding={} Normal=({}, {})",
            aabbA.min.x, aabbA.min.y,
            aabbA.max.x, aabbA.max.y,
            aabbB.min.x, aabbB.min.y,
            aabbB.max.x, aabbB.max.y,
            manifold.IsTouching,
            manifold.IsColliding,
            manifold.Normal.x,
            manifold.Normal.y);

        if (!manifold.IsTouching)
          continue;

        CollisionContact2D contact;
        contact.EntityA = entityA;
        contact.EntityB = entityB;
        contact.Manifold = manifold;
        contact.IsTrigger = colliderA.IsTrigger() || colliderB.IsTrigger();

        m_Contacts.push_back(contact);
      }
    }
  }
}