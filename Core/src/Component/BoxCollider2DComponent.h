#pragma once

#include <glm/glm.hpp>

namespace PicoEngine
{
  class BoxCollider2DComponent
  {
  public:
    glm::vec2 GetOffset() const { return m_Offset; }
    glm::vec2 GetSize() const { return m_Size; }
    bool IsTrigger() const { return m_IsTrigger; }

    void SetOffset(const glm::vec2 &offset) { m_Offset = offset; }
    void SetSize(const glm::vec2 &size) { m_Size = size; }
    void SetIsTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }

  private:
    glm::vec2 m_Offset{0.0f};
    glm::vec2 m_Size{1.0f};
    bool m_IsTrigger = false;
  };
}
