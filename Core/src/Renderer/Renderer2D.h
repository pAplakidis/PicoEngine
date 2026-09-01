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
#include "Camera/OrthographicCamera.h"

class Renderer2D
{
public:
  Renderer2D();
  ~Renderer2D();

  void BeginScene(PicoEngine::OrthographicCamera &camera);
  void EndScene();

  void DrawTriangle(const glm::vec2 &position, float size, const Vec4 &color);

  // TODO: DrawQuad with position and rotation + cleanup: both should end up using the transform matrix version
  void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Vec4 &color);
  // void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Vec4 &color);
  void DrawQuad(const glm::mat4 &transform, const Vec4 &color);

  void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Texture &texture);
  // void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Texture &texture);
  void DrawQuad(const glm::mat4 &transform, const Texture &texture);

  static constexpr uint32_t GetMaxQuads() { return MaxQuads; }

private:
  void Flush();
  float GetTextureIndex(const Texture &texture);

private:
  // TODO: calculate max vertices and indices based on available VRAM
  static constexpr size_t MaxQuads = 10000;
  static constexpr const size_t MaxVertices = 4 * MaxQuads;
  static constexpr const size_t MaxIndices = 6 * MaxQuads;
  static constexpr const size_t MaxTextureSlots = 32; // TODO: query this from OpenGL (?)

  std::vector<Vertex> m_Vertices;
  std::vector<unsigned int> m_Indices;

  std::array<const Texture *, MaxTextureSlots> m_TextureSlots{};
  uint32_t m_TextureSlotIndex = 1; // 0 = white texture, 1+ = actual textures
  std::unique_ptr<Texture> m_WhiteTexture;

  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<Shader> m_Shader;

  glm::mat4 m_ViewProjection{1.0f};
  glm::mat4 m_Transform{1.0f};
};