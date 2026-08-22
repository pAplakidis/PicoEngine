#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "BatchVertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Renderer2D
{
public:
  Renderer2D();
  ~Renderer2D();

  void BeginScene(const glm::mat4 &projection, const glm::mat4 &view);
  void EndScene();
  void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Vec4 &color);
  void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Texture &texture);
  void DrawTriangle(const glm::vec2 &position, float size, const Vec4 &color);

private:
  void Flush();

private:
  // TODO: calculate max vertices and indices based on available VRAM
  const size_t MaxQuads = 10000;
  const size_t MaxVertices = 4 * MaxQuads;
  const size_t MaxIndices = 6 * MaxQuads;
  const size_t MaxTextureSlots = 16; // TODO: query this from OpenGL

  std::vector<Vertex> m_Vertices;
  std::vector<unsigned int> m_Indices;

  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<Shader> m_Shader;

  glm::mat4 m_ViewProjection{1.0f};
  bool m_UseTexture{false};
};