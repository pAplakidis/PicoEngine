#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PicoEngine
{
  class TransformComponent
  {
  public:
    glm::vec3 GetPosition() { return m_Position; }
    float GetRotation() { return m_Rotation; }
    glm::vec3 GetScale() { return m_Scale; }

    glm::mat4 GetTransform() const
    {
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                            glm::scale(glm::mat4(1.0f), m_Scale);
      return transform;
    }

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

    void SetTransform(const glm::mat4 &transform)
    {
      m_Position = glm::vec3(transform[3]);
      m_Rotation = glm::degrees(atan2(transform[1][0], transform[0][0]));
      m_Scale = glm::vec3(glm::length(glm::vec3(transform[0])), glm::length(glm::vec3(transform[1])), glm::length(glm::vec3(transform[2])));
    }

  private:
    glm::vec3 m_Position{0.0f};
    float m_Rotation{0.0f};
    glm::vec3 m_Scale{1.0f};
  };
}
