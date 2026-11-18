#pragma once

#include <glm/glm.hpp>

namespace PicoEngine
{
  enum class BodyType
  {
    Static,   // doesn't move, blocks dynamic bodies
    Dynamic,  // moves using velocity and responds to collisions
    Kinematic // TODO: reserved for later (e.g. moving platforms)
  };

  class RigidBody2DComponent
  {
  public:
    RigidBody2DComponent() = default;

    explicit RigidBody2DComponent(BodyType type)
        : m_BodyType(type) {}

    BodyType GetBodyType() const { return m_BodyType; }
    void SetBodyType(BodyType type) { m_BodyType = type; }

    const glm::vec2 &GetVelocity() const { return m_Velocity; }
    void SetVelocity(const glm::vec2 &velocity) { m_Velocity = velocity; }

    float GetGravityScale() const { return m_GravityScale; }
    void SetGravityScale(float scale) { m_GravityScale = scale; }

  private:
    BodyType m_BodyType = BodyType::Static;

    glm::vec2 m_Velocity{0.0f};
    float m_GravityScale = 1.0f;
  };
}
