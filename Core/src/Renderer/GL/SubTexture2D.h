#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.h"
#include "BatchVertex.h"

namespace PicoEngine
{
  class SubTexture2D
  {
  public:
    SubTexture2D(const std::shared_ptr<Texture> &texture, const glm::vec2 &min, const glm::vec2 &max);

    const std::shared_ptr<Texture> &GetTexture() const { return m_Texture; }
    const Vec2 *GetTexCoords() const { return m_TexCoords; }

    static std::shared_ptr<SubTexture2D> CreateFromCoords(const std::shared_ptr<Texture> &texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize = {1, 1});
    static std::shared_ptr<SubTexture2D> CreateFromPixels(const std::shared_ptr<Texture> &texture, const glm::vec2 &min, const glm::vec2 &max);

  private:
    std::shared_ptr<Texture> m_Texture;
    Vec2 m_TexCoords[4];
  };
}
