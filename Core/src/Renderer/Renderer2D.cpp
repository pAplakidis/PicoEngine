#include "Renderer2D.h"

#include "RendererCommand.h"
#include "GL/VertexBufferLayout.h"

#include "glm/gtc/matrix_transform.hpp"

namespace PicoEngine
{
  bool Renderer2D::s_WireframeEnabled = false;

  // public

  Renderer2D::Renderer2D()
  {
    Init();
  }

  void Renderer2D::Init()
  {
    RendererCommand::EnableBlend();

    m_Vertices.reserve(MaxVertices);
    m_Indices.reserve(MaxIndices);

    uint32_t whitePixel = 0xffffffff;
    m_WhiteTexture = std::make_unique<Texture>(1, 1, &whitePixel);
    m_TextureSlots[0] = m_WhiteTexture.get();

    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertices * sizeof(Vertex));

    VertexBufferLayout layout;
    layout.Push<float>(3, offsetof(Vertex, Position));
    layout.Push<float>(4, offsetof(Vertex, Color));
    layout.Push<float>(2, offsetof(Vertex, TexCoords));
    layout.Push<float>(1, offsetof(Vertex, TexID));

    m_VAO->AddBuffer(*m_VertexBuffer, layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(MaxIndices);

    m_Shader = std::make_unique<Shader>(CORE_RESOURCES_PATH "shaders/basic.vert.glsl",
                                        CORE_RESOURCES_PATH "shaders/basic.frag.glsl");

    int samplers[MaxTextureSlots];

    for (uint32_t i = 0; i < MaxTextureSlots; i++)
    {
      samplers[i] = static_cast<int>(i);
    }

    m_Shader->Bind();
    m_Shader->SetUniform1iv("u_Textures", MaxTextureSlots, samplers);
  }

  void Renderer2D::Shutdown()
  {
  }

  void Renderer2D::BeginScene(OrthographicCamera &camera)
  {
    m_ViewProjection = camera.GetViewProjectionMatrix();
    m_Vertices.clear();
    m_Indices.clear();
    m_TextureSlotIndex = 1;
  }

  void Renderer2D::EndScene()
  {
    Flush();
  }

  void Renderer2D::DrawTriangle(const glm::vec2 &position, float size, const Vec4 &color)
  {
    constexpr uint32_t VertexCount = 3;
    constexpr uint32_t IndexCount = 3;

    if (m_Vertices.size() + VertexCount > MaxVertices ||
        m_Indices.size() + IndexCount > MaxIndices)
    {
      Flush();
    }

    uint32_t offset = static_cast<uint32_t>(m_Vertices.size());
    const float halfSize = size * 0.5f;
    m_Vertices.push_back({{position.x, position.y + halfSize, 0.0f}, color, {0.5f, 1.0f}, 0.0f});
    m_Vertices.push_back({{position.x + halfSize, position.y - halfSize, 0.0f}, color, {1.0f, 0.0f}, 0.0f});
    m_Vertices.push_back({{position.x - halfSize, position.y - halfSize, 0.0f}, color, {0.0f, 0.0f}, 0.0f});
    m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2});
  }

  void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Vec4 &color)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawQuad(transform, color);
  }

  void Renderer2D::DrawQuad(const glm::vec2 &position, float rotation, const glm::vec2 &size, const Vec4 &color)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawQuad(transform, color);
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

    static constexpr Vec2 texCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    for (int i = 0; i < 4; i++)
    {
      glm::vec4 transformed = transform * localPositions[i];
      m_Vertices.push_back({{transformed.x, transformed.y, transformed.z}, color, texCoords[i], 0.0f});
    }
    m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2, offset + 2, offset + 3, offset + 0});
  }

  void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const SubTexture2D &subTexture)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawQuad(transform, subTexture);
  }

  void Renderer2D::DrawQuad(const glm::vec2 &position, float rotation, const glm::vec2 &size, const SubTexture2D &subTexture)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawQuad(transform, subTexture);
  }

  void Renderer2D::DrawQuad(const glm::mat4 &transform, const SubTexture2D &subTexture)
  {
    constexpr uint32_t VertexCount = 4;
    constexpr uint32_t IndexCount = 6;

    if (m_Vertices.size() + VertexCount > MaxVertices || m_Indices.size() + IndexCount > MaxIndices)
    {
      Flush();
    }

    const auto &texture = subTexture.GetTexture();
    const Vec2 *texCoords = subTexture.GetTexCoords();

    float textureIndex = GetTextureIndex(*texture);

    uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

    static constexpr glm::vec4 localPositions[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f}};

    for (int i = 0; i < 4; i++)
    {
      glm::vec4 transformed = transform * localPositions[i];

      m_Vertices.push_back({{transformed.x, transformed.y, transformed.z},
                            Vec4{1.0f, 1.0f, 1.0f, 1.0f},
                            texCoords[i],
                            textureIndex});
    }

    m_Indices.insert(m_Indices.end(), {offset + 0, offset + 1, offset + 2,
                                       offset + 2, offset + 3, offset + 0});
  }

  void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Texture &texture)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawQuad(transform, texture);
  }

  void Renderer2D::DrawQuad(const glm::vec2 &position, float rotation, const glm::vec2 &size, const Texture &texture)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawQuad(transform, texture);
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

    static constexpr glm::vec4 localPositions[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f}};
    static constexpr Vec2 texCoords[4] = {
        {0.0f, 0.0f},  // bottom left
        {1.0f, 0.0f},  // bottom right
        {1.0f, 1.0f},  // top right
        {0.0f, 1.0f}}; // top left

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

  // private

  void Renderer2D::Flush()
  {
    if (m_Vertices.empty())
      return;

    m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    m_IndexBuffer->SetData(m_Indices.data(), static_cast<unsigned int>(m_Indices.size()));

    for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
    {
      m_TextureSlots[i]->Bind(i);
    }

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_ViewProjection", m_ViewProjection);
    m_IndexBuffer->Bind();

    RendererCommand::SetWireframe(s_WireframeEnabled);
    RendererCommand::DrawIndexed(*m_VAO, static_cast<unsigned int>(m_Indices.size()));

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
}
