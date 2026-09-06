#pragma once

#include <memory>

#include "RendererAPI.h"
#include "GL/VertexArray.h"

namespace PicoEngine
{

  class RendererCommand
  {
  public:
    static void Init();

    static void Clear();
    static void SetClearColor(float r, float g, float b, float a);

    static void SetViewport(int x, int y, int width, int height);
    static void DrawIndexed(const VertexArray &vertexArray, unsigned int count);

    static void EnableBlend();
    static void SetWireframe(bool enabled = true);

  private:
    static std::unique_ptr<RendererAPI> s_RendererAPI;
  };

}