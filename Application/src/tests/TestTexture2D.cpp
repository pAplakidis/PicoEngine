#include "TestTexture2D.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer/Renderer.h"
#include "GLCore/OpenGLDebug.h"
#include "Application.h"

namespace test
{
    TestTexture2D::TestTexture2D()
        : m_Renderer2D(PicoEngine::Renderer::GetRenderer2D()),
          m_CameraController(
              static_cast<float>(PicoEngine::Application::Get().GetWindow().GetWidth()) /
              static_cast<float>(PicoEngine::Application::Get().GetWindow().GetHeight())),
          m_TranslationA(-0.5f, 0.0f, 0.0f),
          m_TranslationB(0.5f, 0.0f, 0.0f)
    {

        m_Texture = std::make_unique<Texture>(APPLICATION_RESOURCES_PATH "textures/gold-dollar.png");
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
        m_CameraController.OnUpdate(deltaTime);
    }

    void TestTexture2D::OnRender()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Renderer2D.BeginScene(m_CameraController.GetCamera());

        m_Renderer2D.DrawQuad(
            {m_TranslationA.x, m_TranslationA.y},
            {0.5f, 0.5f},
            *m_Texture);

        m_Renderer2D.DrawQuad(
            {m_TranslationB.x, m_TranslationB.y},
            {0.5f, 0.5f},
            *m_Texture);

        m_Renderer2D.EndScene();
    }

    void TestTexture2D::OnEvent(PicoEngine::Event &event)
    {
        m_CameraController.OnEvent(event);
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, -2.0f, 2.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, -2.0f, 2.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
