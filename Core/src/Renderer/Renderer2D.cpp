#include "Renderer2D.h"

#include <iostream>

#include "Geometry/PrimitiveFactory.h"
#include "VertexBufferLayout.h"

Renderer2D::Renderer2D()
{
  m_Vertices.reserve(MaxVertices);
  m_Indices.reserve(MaxIndices);

  m_VAO = std::make_unique<VertexArray>();
  m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertices * sizeof(Vertex));

  VertexBufferLayout layout;
  layout.Push<float>(3, offsetof(Vertex, Position));
  layout.Push<float>(4, offsetof(Vertex, Color));
  layout.Push<float>(2, offsetof(Vertex, TexCoords));
  layout.Push<float>(1, offsetof(Vertex, TexID));

  m_VAO->AddBuffer(*m_VertexBuffer, layout);

  m_IndexBuffer = std::make_unique<IndexBuffer>(MaxIndices);

  m_Shader = std::make_unique<Shader>(
      CORE_RESOURCES_PATH "shaders/basic.vert.glsl",
      CORE_RESOURCES_PATH "shaders/basic.frag.glsl");

  m_Shader->Bind();
  m_Shader->SetUniform1i("u_UseTexture", 0);
  m_Shader->SetUniform4f("u_Color", glm::vec4(1.0f));
}

Renderer2D::~Renderer2D()
{
}

void Renderer2D::BeginScene(const glm::mat4 &projection, const glm::mat4 &view)
{
  m_ViewProjection = projection * view;
  m_Vertices.clear();
  m_Indices.clear();
  m_UseTexture = false;
}

void Renderer2D::EndScene()
{
  Flush();
}

void Renderer2D::DrawTriangle(const glm::vec2 &position, float size, const Vec4 &color)
{
  constexpr uint32_t VertexCount = 3;
  constexpr uint32_t IndexCount = 3;

  if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  auto triangle = PrimitiveFactory::CreateTriangle(position.x, position.y, size, color, -1.0f);

  m_Vertices.insert(
      m_Vertices.end(),
      triangle.begin(),
      triangle.end());

  m_Indices.push_back(offset + 0);
  m_Indices.push_back(offset + 1);
  m_Indices.push_back(offset + 2);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Vec4 &color)
{
  constexpr uint32_t VertexCount = 4;
  constexpr uint32_t IndexCount = 6;

  if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  auto quad = PrimitiveFactory::CreateQuad(position.x, position.y, size.x, size.y, color, -1.0f);

  m_Vertices.insert(m_Vertices.end(), quad.begin(), quad.end());

  m_Indices.push_back(offset + 0);
  m_Indices.push_back(offset + 1);
  m_Indices.push_back(offset + 2);

  m_Indices.push_back(offset + 2);
  m_Indices.push_back(offset + 3);
  m_Indices.push_back(offset + 0);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Texture &texture)
{
  constexpr uint32_t VertexCount = 4;
  constexpr uint32_t IndexCount = 6;

  if (m_Vertices.size() + VertexCount > MaxVertices ||
      m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  // TODO: for now every textured quad in batch uses texture slot 0.
  constexpr float textureID = 0.0f;

  auto quad = PrimitiveFactory::CreateQuad(
      position.x,
      position.y,
      size.x,
      size.y,
      Vec4{1.0f, 1.0f, 1.0f, 1.0f},
      textureID);

  m_Vertices.insert(
      m_Vertices.end(),
      quad.begin(),
      quad.end());

  m_Indices.push_back(offset + 0);
  m_Indices.push_back(offset + 1);
  m_Indices.push_back(offset + 2);

  m_Indices.push_back(offset + 2);
  m_Indices.push_back(offset + 3);
  m_Indices.push_back(offset + 0);

  m_UseTexture = true;

  texture.Bind(0);
}

void Renderer2D::Flush()
{
  if (m_Vertices.empty())
    return;

  m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
  m_IndexBuffer->SetData(m_Indices.data(), static_cast<unsigned int>(m_Indices.size()));

  m_Shader->Bind();
  m_Shader->SetUniformMat4f("u_MVP", m_ViewProjection);
  m_Shader->SetUniform1i("u_UseTexture", m_UseTexture ? 1 : 0);
  m_Shader->SetUniform1i("u_Texture", 0);

  m_VAO->Bind();
  m_IndexBuffer->Bind();
  GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));

  m_Vertices.clear();
  m_Indices.clear();
}