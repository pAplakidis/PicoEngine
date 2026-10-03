#pragma once

#include <glm/glm.hpp>

namespace PicoEngine
{
  class TransformComponent
  {
  public:
    glm::vec3 GetPosition() { return m_Position; }
    float GetRotation() { return m_Rotation; }
    glm::vec3 GetScale() { return m_Scale; }

    void SetPosition(const glm::vec3 &position)
    {
      m_Position = position;
    }

    void SetRotation(float rotation)
    {
      m_Rotation = rotation;
    }

    void SetScale(const glm::vec3 &scale)
    {
      m_Scale = scale;
    }

  private:
    glm::vec3 m_Position{0.0f};
    float m_Rotation{0.0f};
    glm::vec3 m_Scale{1.0f};
  };
}
