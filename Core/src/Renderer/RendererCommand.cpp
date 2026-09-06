#include "RendererCommand.h"

#include "GL/OpenGLRendererAPI.h"

namespace PicoEngine
{

  std::unique_ptr<RendererAPI> RendererCommand::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();

  void RendererCommand::Init()
  {
    s_RendererAPI->Init();
  }

  void RendererCommand::Clear()
  {
    s_RendererAPI->Clear();
  }

  void RendererCommand::SetClearColor(float r, float g, float b, float a)
  {
    s_RendererAPI->SetClearColor(r, g, b, a);
  }

  void RendererCommand::SetViewport(int x, int y, int width, int height)
  {
    s_RendererAPI->SetViewport(x, y, width, height);
  }

  void RendererCommand::DrawIndexed(const VertexArray &vertexArray, unsigned int count)
  {
    s_RendererAPI->DrawIndexed(vertexArray, count);
  }

  void RendererCommand::EnableBlend()
  {
    s_RendererAPI->EnableBlend();
  }

  void RendererCommand::SetWireframe(bool enabled)
  {
    s_RendererAPI->SetWireframe(enabled);
  }

}