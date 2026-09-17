#include "TestTriangle.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer/Renderer.h"
#include "GLCore/OpenGLDebug.h"
#include "Application.h"

namespace test
{
    TestTriangle::TestTriangle()
        : m_Renderer2D(PicoEngine::Renderer::GetRenderer2D()),
          m_CameraController(
              static_cast<float>(PicoEngine::Application::Get().GetWindow().GetWidth()) /
              static_cast<float>(PicoEngine::Application::Get().GetWindow().GetHeight())),
          m_TranslationQ0(-0.6f, 0.0f, 0.0f),
          m_TranslationQ1(0.6f, 0.0f, 0.0f),
          m_TranslationT0(0.0f, 0.0f, 0.0f)
    {
    }

    TestTriangle::~TestTriangle()
    {
    }

    void TestTriangle::OnUpdate(float deltaTime)
    {
        m_CameraController.OnUpdate(deltaTime);
    }

    void TestTriangle::OnRender()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Renderer2D.BeginScene(m_CameraController.GetCamera());
        m_Renderer2D.SetWireframeEnabled(m_WireframeEnabled);

        m_Renderer2D.DrawQuad(
            glm::vec2(m_TranslationQ0),
            glm::vec2(0.4f, 0.4f),
            Vec4{1.0f, 0.0f, 0.0f, 1.0f});

        m_Renderer2D.DrawTriangle(
            glm::vec2(m_TranslationT0),
            0.4f,
            Vec4{0.0f, 1.0f, 0.0f, 1.0f});

        m_Renderer2D.DrawQuad(
            glm::vec2(m_TranslationQ1),
            glm::vec2(0.4f, 0.4f),
            Vec4{0.0f, 0.0f, 1.0f, 1.0f});

        m_Renderer2D.EndScene();
    }

    void TestTriangle::OnEvent(PicoEngine::Event &event)
    {
        m_CameraController.OnEvent(event);
    }

    void TestTriangle::OnImGuiRender()
    {
        ImGui::SliderFloat3("Quad 0 Translation", &m_TranslationQ0.x, -2.0f, 2.0f);
        ImGui::SliderFloat3("Triangle Translation", &m_TranslationT0.x, -2.0f, 2.0f);
        ImGui::SliderFloat3("Quad 1 Translation", &m_TranslationQ1.x, -2.0f, 2.0f);
        ImGui::Checkbox("Wireframe Mode", &m_WireframeEnabled);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}