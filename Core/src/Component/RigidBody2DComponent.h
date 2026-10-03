#pragma once

#include <glm/glm.hpp>

namespace PicoEngine
{
  enum class BodyType
  {
    Static,
    Dynamic,
    Kinematic
  };

  class RigidBody2DComponent
  {
  private:
    BodyType m_BodyType = BodyType::Static;

    glm::vec2 m_Velocity{0.0f};
    float GravityScale = 1.0f;
  };
}
