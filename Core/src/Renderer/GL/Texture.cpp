#include "Texture.h"

#include <GL/glew.h>

#include "Util/Log.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string &path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),
      m_Height(0), m_BPP(0)
{
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer =
      stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 4 => RGBA

  if (m_LocalBuffer == nullptr)
  {
    LOG_ERROR("Failed to load texture: {}", path);
    return;
  }

  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  // TODO: select between Linear and Nearest (?)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // TODO: select between Repeat, MirroredRepeat, ClampToEdge and ClampToBorder
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // TODO: use and enable/disable this
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
  // GL_LINEAR_MIPMAP_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, m_LocalBuffer);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (m_LocalBuffer)
    stbi_image_free(m_LocalBuffer);
}

Texture::Texture(uint32_t width, uint32_t height, const void *data)
    : m_RendererID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(width),
      m_Height(height), m_BPP(4) // 4 => RGBA
{
  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
  if (m_RendererID != 0)
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
  if (m_RendererID == 0)
    return;
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
  if (m_RendererID == 0)
    return;
  glBindTexture(GL_TEXTURE_2D, 0);
}
