#pragma once

#include <vector>
#include <entt/entt.hpp>

#include "Physics/CollisionContact2D.h"

namespace PicoEngine
{
  class CollisionSystem2D
  {
  public:
    void Update(entt::registry &registry);

    const std::vector<CollisionContact2D> &GetContacts() const { return m_Contacts; };

    SweepResult2D SweepStatic(entt::registry &registry, entt::entity movingEntity, const glm::vec2 &displacement) const;

  private:
    std::vector<CollisionContact2D> m_Contacts;
  };
}
