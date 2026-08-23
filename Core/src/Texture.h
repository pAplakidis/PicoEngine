#pragma once

#include "Renderer.h"

class Texture
{
private:
  unsigned int m_RendererID;
  std::string m_FilePath;
  unsigned char *m_LocalBuffer;
  int m_Width, m_Height, m_BPP;

public:
  Texture(const std::string &path);
  Texture(uint32_t width, uint32_t height, const void *data);
  ~Texture();

  void Bind(unsigned int slot = 0) const;
  void Unbind() const;
  unsigned int GetRendererID() const { return m_RendererID; };

  inline int GetWidth() const { return m_Width; }
  inline int GetHeight() const { return m_Height; }
};
