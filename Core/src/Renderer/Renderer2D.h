#pragma once

#include <glm/glm.hpp>

class Renderer2D
{
public:
  Renderer2D();
  ~Renderer2D();

  void BeginScene(const glm::mat4 &projection, const glm::mat4 &view);
  void EndScene();
  void DrawTriangle(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
  void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
};
