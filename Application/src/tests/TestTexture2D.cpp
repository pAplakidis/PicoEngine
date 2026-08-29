#include "TestTexture2D.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLCore/OpenGLDebug.h"

namespace test
{
    TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f))),
          m_TranslationA(200.0f, 200.0f, 0.0f),
          m_TranslationB(400.0f, 200.0f, 0.0f)
    {
        m_Renderer2D = std::make_unique<Renderer2D>();
        m_Texture = std::make_unique<Texture>(APPLICATION_RESOURCES_PATH "textures/gold-dollar.png");
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Renderer2D->BeginScene(m_Proj, m_View);

        m_Renderer2D->DrawQuad(
            {m_TranslationA.x, m_TranslationA.y},
            {100.0f, 100.0f},
            *m_Texture);

        m_Renderer2D->DrawQuad(
            {m_TranslationB.x, m_TranslationB.y},
            {100.0f, 100.0f},
            *m_Texture);

        m_Renderer2D->EndScene();
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
