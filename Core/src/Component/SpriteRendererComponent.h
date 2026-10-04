#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Renderer/GL/Texture.h"
#include "Renderer/GL/SubTexture2D.h"

namespace PicoEngine
{
  class SpriteRendererComponent
  {
  public:
    SpriteRendererComponent() = default;

    SpriteRendererComponent(std::shared_ptr<Texture> texture)
        : m_Texture(std::move(texture))
    {
    }

    SpriteRendererComponent(const std::shared_ptr<SubTexture2D> &subTexture)
        : m_SubTexture(subTexture),
          m_Texture(subTexture->GetTexture())
    {
    }

    std::shared_ptr<Texture> GetTexture() { return m_Texture; }
    const std::shared_ptr<SubTexture2D> &GetSubTexture() const { return m_SubTexture; }
    bool HasSubTexture() const { return m_SubTexture != nullptr; }

    glm::vec4 GetColor() { return m_Color; }

    void SetTexture(const std::shared_ptr<Texture> &texture) { m_Texture = texture; }
    void SetColor(const glm::vec4 &color) { m_Color = color; }

  private:
    std::shared_ptr<Texture> m_Texture;
    std::shared_ptr<SubTexture2D> m_SubTexture;
    glm::vec4 m_Color{1.0f};

    glm::vec2 UVMin{0.0f, 0.0f};
    glm::vec2 UVMax{1.0f, 1.0f};
  };
}
