#pragma once

#include <glm/glm.hpp>

namespace PicoEngine
{
  class BoxCollider2DComponent
  {
  private:
    glm::vec2 m_Offset{0.0f};
    glm::vec2 m_Size{1.0f};

    bool m_IsTrigger = false;
  };
}
