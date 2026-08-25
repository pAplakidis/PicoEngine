#include "TestTransformations.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer/Renderer2D.h"
#include "GLCore/OpenGLDebug.h"

namespace test
{
  TestTransformations::TestTransformations()
      : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f))),
        m_Translation(200.0f, 200.0f, 0.0f),
        m_Rotation(0.0f, 0.0f, 0.0f),
        m_Scale(100.0f, 100.0f, 100.0f)
  {
    m_Renderer2D = std::make_unique<Renderer2D>();
    m_Texture = std::make_unique<Texture>(APPLICATION_RESOURCES_PATH "textures/mario.png");
  }

  TestTransformations::~TestTransformations()
  {
  }

  void TestTransformations::OnUpdate(float deltaTime)
  {
  }

  void TestTransformations::OnRender()
  {

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Renderer2D->BeginScene(m_Proj, m_View);

    // TODO: do this inside DrawQuad (?)
    // T * R * S
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), m_Translation) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(glm::mat4(1.0f), m_Scale);

    m_Renderer2D->DrawQuad(transform, *m_Texture);

    m_Renderer2D->EndScene();
  }

  void TestTransformations::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 960.0f);
    ImGui::SliderFloat("Rotation", &m_Rotation.z, -180.0f, 180.0f);
    ImGui::SliderFloat2("Scale", &m_Scale.x, 10.0f, 1000.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

}
