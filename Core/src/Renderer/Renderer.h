#pragma once

#include <memory>
#include "Renderer2D.h"
#include "GL/VertexArray.h"
#include "GL/Shader.h"

namespace PicoEngine
{

  class Renderer
  {
  public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(int width, int height);

    static Renderer2D &GetRenderer2D();

  private:
    static std::unique_ptr<Renderer2D> s_Renderer2D;
  };
}