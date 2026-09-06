#include "OpenGLRendererAPI.h"

namespace PicoEngine
{

  void OpenGLRendererAPI::Init()
  {
  }

  void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
  {
    glClearColor(r, g, b, a);
  }

  void OpenGLRendererAPI::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
  {
    glViewport(x, y, width, height);
  }

  void OpenGLRendererAPI::DrawIndexed(const VertexArray &vertexArray, unsigned int count)
  {
    vertexArray.Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  }

  void OpenGLRendererAPI::EnableBlend()
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void OpenGLRendererAPI::SetWireframe(bool enabled)
  {
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
  }

}