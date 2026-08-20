#include "TestTriangle.h"

#include <vector>

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "BatchVertex.h"
#include "Geometry/PrimitiveFactory.h"

namespace test
{
  TestTriangle::TestTriangle()
      : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
        m_TranslationQ0(200, 200, 0),
        m_TranslationQ1(500, 200, 0),
        m_TranslationT0(350, 200, 0)
  {
    unsigned int indices[] = {
        // Quad 0
        0, 1, 2,
        2, 3, 0,

        // Triangle
        4, 5, 6,

        // Quad 1
        7, 8, 9,
        9, 10, 7};

    m_VAO = std::make_unique<VertexArray>();

    m_VertexBuffer = std::make_unique<VertexBuffer>(
        nullptr,
        sizeof(Vertex) * 1000);

    VertexBufferLayout layout;
    layout.Push<float>(3, offsetof(Vertex, Position));
    layout.Push<float>(4, offsetof(Vertex, Color));
    layout.Push<float>(2, offsetof(Vertex, TexCoords));
    layout.Push<float>(1, offsetof(Vertex, TexID));

    m_VAO->AddBuffer(*m_VertexBuffer, layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(
        indices,
        15);

    m_Shader = std::make_unique<Shader>(
        CORE_RESOURCES_PATH "shaders/basic.vert.glsl",
        CORE_RESOURCES_PATH "shaders/basic.frag.glsl");

    m_Shader->Bind();
    m_Shader->SetUniform4f(
        "u_Color",
        glm::vec4(1.0f));

    m_Shader->SetUniform1i(
        "u_UseTexture",
        0);
  }

  TestTriangle::~TestTriangle()
  {
  }

  void TestTriangle::OnUpdate(float deltaTime)
  {
  }

  void TestTriangle::OnRender()
  {
    GLCall(glClearColor(
        0.0f,
        0.0f,
        0.0f,
        1.0f));

    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    auto q0 = PrimitiveFactory::CreateQuad(
        m_TranslationQ0.x,
        m_TranslationQ0.y,
        100.0f,
        100.0f,
        Vec4{1.0f, 0.0f, 0.0f, 1.0f},
        -1.0f);

    auto t0 = PrimitiveFactory::CreateTriangle(
        m_TranslationT0.x,
        m_TranslationT0.y,
        100.0f,
        Vec4{0.0f, 1.0f, 0.0f, 1.0f},
        -1.0f);

    auto q1 = PrimitiveFactory::CreateQuad(
        m_TranslationQ1.x,
        m_TranslationQ1.y,
        100.0f,
        100.0f,
        Vec4{0.0f, 0.0f, 1.0f, 1.0f},
        -1.0f);

    std::vector<Vertex> vertices;
    vertices.reserve(11);

    vertices.insert(vertices.end(), q0.begin(), q0.end());
    vertices.insert(vertices.end(), t0.begin(), t0.end());
    vertices.insert(vertices.end(), q1.begin(), q1.end());

    m_VertexBuffer->SetData(
        vertices.data(),
        vertices.size() * sizeof(Vertex));

    glm::mat4 mvp = m_Proj * m_View;

    m_Shader->Bind();
    m_Shader->SetUniformMat4f(
        "u_MVP",
        mvp);

    Renderer renderer;

    renderer.Draw(
        *m_VAO,
        *m_IndexBuffer,
        *m_Shader);
  }

  void TestTriangle::OnImGuiRender()
  {
    ImGui::SliderFloat3(
        "Quad 0 Translation",
        &m_TranslationQ0.x,
        0.0f,
        960.0f);

    ImGui::SliderFloat3(
        "Triangle Translation",
        &m_TranslationT0.x,
        0.0f,
        960.0f);

    ImGui::SliderFloat3(
        "Quad 1 Translation",
        &m_TranslationQ1.x,
        0.0f,
        960.0f);

    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate,
        ImGui::GetIO().Framerate);
  }
}
