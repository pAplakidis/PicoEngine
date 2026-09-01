#include "Renderer2D.h"

#include "Geometry/PrimitiveFactory.h"
#include "VertexBufferLayout.h"

// TODO: cleanup - use this
// struct RendererData
// {
//   GLuint QuadVA = 0;
//   GLuint QuadVB = 0;
//   GLuint QuadIB = 0;

//   GLuint WhiteTexture = 0;
//   uint32_t WhiteTextureSlot = 0;

//   uint32_t IndexCount = 0;

//   Vertex *QuadBuffer = nullptr;
//   Vertex *QuadBufferPtr = nullptr;

//   std::array<uint32_t, MaxTextureSlots> TextureSlots;
//   uint32_t TextureSlotIndex = 1;

//   // Renderer2D::Stats RendererStats;
// }

Renderer2D::Renderer2D()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_Vertices.reserve(MaxVertices);
  m_Indices.reserve(MaxIndices);

  uint32_t whitePixel = 0xffffffff;
  m_WhiteTexture = std::make_unique<Texture>(1, 1, &whitePixel);
  m_TextureSlots[0] = m_WhiteTexture.get();

  m_VAO = std::make_unique<VertexArray>();
  m_VertexBuffer =
      std::make_unique<VertexBuffer>(nullptr, MaxVertices * sizeof(Vertex));

  VertexBufferLayout layout;
  layout.Push<float>(3, offsetof(Vertex, Position));
  layout.Push<float>(4, offsetof(Vertex, Color));
  layout.Push<float>(2, offsetof(Vertex, TexCoords));
  layout.Push<float>(1, offsetof(Vertex, TexID));

  m_VAO->AddBuffer(*m_VertexBuffer, layout);

  m_IndexBuffer = std::make_unique<IndexBuffer>(MaxIndices);

  m_Shader =
      std::make_unique<Shader>(CORE_RESOURCES_PATH "shaders/basic.vert.glsl",
                               CORE_RESOURCES_PATH "shaders/basic.frag.glsl");

  int samplers[MaxTextureSlots];

  for (uint32_t i = 0; i < MaxTextureSlots; i++)
  {
    samplers[i] = static_cast<int>(i);
  }

  m_Shader->Bind();
  m_Shader->SetUniform1iv("u_Textures", MaxTextureSlots, samplers);
}

Renderer2D::~Renderer2D() {}

void Renderer2D::BeginScene(PicoEngine::OrthographicCamera &camera)
{
  m_ViewProjection = camera.GetViewProjectionMatrix();
  m_Vertices.clear();
  m_Indices.clear();
}

void Renderer2D::EndScene() { Flush(); }

void Renderer2D::DrawTriangle(const glm::vec2 &position, float size,
                              const Vec4 &color)
{
  constexpr uint32_t VertexCount = 3;
  constexpr uint32_t IndexCount = 3;

  if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  auto triangle = PrimitiveFactory::CreateTriangle(position.x, position.y, size,
                                                   color, 0.0f);

  m_Vertices.insert(m_Vertices.end(), triangle.begin(), triangle.end());

  m_Indices.push_back(offset + 0);
  m_Indices.push_back(offset + 1);
  m_Indices.push_back(offset + 2);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const Vec4 &color)
{
  constexpr uint32_t VertexCount = 4;
  constexpr uint32_t IndexCount = 6;

  if (m_Vertices.size() + VertexCount > MaxVertices ||
      m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());
  auto quad = PrimitiveFactory::CreateQuad(
      position.x, position.y, size.x, size.y, color, 0.0f); // white texture
  m_Vertices.insert(m_Vertices.end(), quad.begin(), quad.end());
  m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2,
                                     offset + 2, offset + 3, offset + 0});
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const Texture &texture)
{
  constexpr uint32_t VertexCount = 4;
  constexpr uint32_t IndexCount = 6;

  if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  float textureIndex = GetTextureIndex(texture);

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  auto quad =
      PrimitiveFactory::CreateQuad(position.x, position.y, size.x, size.y, Vec4{1.0f, 1.0f, 1.0f, 1.0f}, textureIndex);

  m_Vertices.insert(m_Vertices.end(), quad.begin(), quad.end());
  m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2,
                                     offset + 2, offset + 3, offset + 0});
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Vec4 &color)
{
  constexpr uint32_t VertexCount = 4;
  constexpr uint32_t IndexCount = 6;

  if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  static constexpr glm::vec4 localPositions[4] = {{-0.5f, -0.5f, 0.0f, 1.0f},
                                                  {0.5f, -0.5f, 0.0f, 1.0f},
                                                  {0.5f, 0.5f, 0.0f, 1.0f},
                                                  {-0.5f, 0.5f, 0.0f, 1.0f}};

  static constexpr Vec2 texCoords[4] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  for (int i = 0; i < 4; i++)
  {
    glm::vec4 transformed = transform * localPositions[i];
    m_Vertices.push_back({{transformed.x, transformed.y, transformed.z},
                          color,
                          texCoords[i],
                          0.0f});
  }
  m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2,
                                     offset + 2, offset + 3, offset + 0});
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Texture &texture)
{
  constexpr uint32_t VertexCount = 4;
  constexpr uint32_t IndexCount = 6;

  if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
  {
    Flush();
  }

  float textureIndex = GetTextureIndex(texture);

  uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

  static constexpr glm::vec4 localPositions[4] = {{-0.5f, -0.5f, 0.0f, 1.0f},
                                                  {0.5f, -0.5f, 0.0f, 1.0f},
                                                  {0.5f, 0.5f, 0.0f, 1.0f},
                                                  {-0.5f, 0.5f, 0.0f, 1.0f}};

  static constexpr Vec2 texCoords[4] = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  for (int i = 0; i < 4; i++)
  {
    glm::vec4 transformed = transform * localPositions[i];
    m_Vertices.push_back({{transformed.x, transformed.y, transformed.z},
                          Vec4{1, 1, 1, 1},
                          texCoords[i],
                          textureIndex});
  }
  m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2,
                                     offset + 2, offset + 3, offset + 0});
}

void Renderer2D::Flush()
{
  if (m_Vertices.empty())
    return;

  m_VertexBuffer->SetData(m_Vertices.data(),
                          m_Vertices.size() * sizeof(Vertex));
  m_IndexBuffer->SetData(m_Indices.data(),
                         static_cast<unsigned int>(m_Indices.size()));

  for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
  {
    m_TextureSlots[i]->Bind(i);
  }

  m_Shader->Bind();
  m_Shader->SetUniformMat4f("u_ViewProjection", m_ViewProjection);

  m_VAO->Bind();
  m_IndexBuffer->Bind();

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()),
                 GL_UNSIGNED_INT, nullptr);

  m_Vertices.clear();
  m_Indices.clear();

  m_TextureSlotIndex = 1;
}

float Renderer2D::GetTextureIndex(const Texture &texture)
{
  for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
  {
    if (m_TextureSlots[i]->GetRendererID() == texture.GetRendererID())
    {
      return static_cast<float>(i);
    }
  }

  if (m_TextureSlotIndex >= MaxTextureSlots)
  {
    Flush();
  }

  uint32_t slot = m_TextureSlotIndex;

  m_TextureSlots[slot] = &texture;
  m_TextureSlotIndex++;

  return static_cast<float>(slot);
}
