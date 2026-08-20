#include "TestTriangle.h"

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
        m_Translation(200, 200, 0)
  {
    auto q0 = PrimitiveFactory::CreateQuad(
        0.0f,
        0.0f,
        100.0f,
        100.0f,
        Vec4{1.0f, 1.0f, 1.0f, 1.0f},
        -1.0f);
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000); // TODO: more generic max vertex count
    VertexBufferLayout layout;
    layout.Push<float>(3, offsetof(Vertex, Position));
    layout.Push<float>(4, offsetof(Vertex, Color));
    layout.Push<float>(2, offsetof(Vertex, TexCoords));
    layout.Push<float>(1, offsetof(Vertex, TexID));
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
    m_VertexBuffer->SetData(q0.data(), q0.size() * sizeof(Vertex));

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    m_Shader = std::make_unique<Shader>(CORE_RESOURCES_PATH "shaders/basic.vert.glsl", CORE_RESOURCES_PATH "shaders/basic.frag.glsl");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", glm::vec4(0.8f, 0.3f, 0.8f, 1.0f));
    m_Shader->SetUniform1i("u_UseTexture", 0);
  }

  TestTriangle::~TestTriangle()
  {
  }

  void TestTriangle::OnUpdate(float deltaTime)
  {
  }

  void TestTriangle::OnRender()
  {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }
  }

  void TestTriangle::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}
