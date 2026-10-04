#include "SubTexture2D.h"

namespace PicoEngine
{
  SubTexture2D::SubTexture2D(const std::shared_ptr<Texture> &texture, const glm::vec2 &min, const glm::vec2 &max)
      : m_Texture(texture),
        m_TexCoords{
            {min.x, min.y},
            {max.x, min.y},
            {max.x, max.y},
            {min.x, max.y}}
  {
  }

  std::shared_ptr<SubTexture2D> SubTexture2D::CreateFromCoords(const std::shared_ptr<Texture> &texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize = {1, 1})
  {
    const float sheetWidth = static_cast<float>(texture->GetWidth());
    const float sheetHeight = static_cast<float>(texture->GetHeight());

    glm::vec2 min = {
        (coords.x * cellSize.x) / sheetWidth,
        1.0f - ((coords.y + spriteSize.y) * cellSize.y) / sheetHeight};
    glm::vec2 max = {
        ((coords.x + spriteSize.x) * cellSize.x) / sheetWidth,
        1.0f - (coords.y * cellSize.y) / sheetHeight};
    return std::make_shared<SubTexture2D>(texture, min, max);
  }

  std::shared_ptr<SubTexture2D> SubTexture2D::CreateFromPixels(const std::shared_ptr<Texture> &texture, const glm::vec2 &min, const glm::vec2 &max)
  {
    glm::vec2 normalizedMin = {
        min.x / static_cast<float>(texture->GetWidth()),
        min.y / static_cast<float>(texture->GetHeight())};
    glm::vec2 normalizedMax = {
        max.x / static_cast<float>(texture->GetWidth()),
        max.y / static_cast<float>(texture->GetHeight())};
    return std::make_shared<SubTexture2D>(
        texture,
        normalizedMin,
        normalizedMax);
  }
}
