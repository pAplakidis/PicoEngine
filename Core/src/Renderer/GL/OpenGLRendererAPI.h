#pragma once

#include "Renderer/RendererAPI.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace PicoEngine
{
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    void Init() override;

    void SetClearColor(float r, float g, float b, float a) override;
    void Clear() override;

    void SetViewport(int x, int y, int width, int height) override;
    void DrawIndexed(const VertexArray &vertexArray,
                     unsigned int count) override;

    void EnableBlend() override;
    void SetWireframe(bool enabled = true) override;
  };
}