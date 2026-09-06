#pragma once

#include "VertexArray.h"

class RendererAPI
{
public:
  virtual ~RendererAPI() = default;

  virtual void Init() = 0;

  virtual void SetClearColor(float r, float g, float b, float a) = 0;
  virtual void Clear() = 0;

  virtual void SetViewport(int x, int y, int width, int height) = 0;
  virtual void DrawIndexed(const VertexArray &vertexArray, unsigned int count) = 0;

  virtual void EnableBlend() = 0;
  virtual void SetWireframe(bool enabled = true) = 0;
};
