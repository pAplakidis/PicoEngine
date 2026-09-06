#include "Renderer.h"

#include "RendererCommand.h"

namespace PicoEngine
{
  std::unique_ptr<Renderer2D> Renderer::s_Renderer2D;

  void Renderer::Init()
  {
    RendererCommand::Init();

    s_Renderer2D = std::make_unique<Renderer2D>();
  }

  void Renderer::Shutdown()
  {
    s_Renderer2D.reset();
  }

  void Renderer::OnWindowResize(int width, int height)
  {
    RendererCommand::SetViewport(0, 0, width, height);
  }

  Renderer2D &Renderer::GetRenderer2D()
  {
    return *s_Renderer2D;
  }
}