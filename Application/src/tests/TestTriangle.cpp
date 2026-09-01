#include "TestTriangle.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer/Renderer2D.h"
#include "GLCore/OpenGLDebug.h"

namespace test
{
    TestTriangle::TestTriangle()
        : m_Camera(0.0f, 960.0f, 0.0f, 540.0f),
          m_TranslationQ0(200.0f, 200.0f, 0.0f),
          m_TranslationQ1(500.0f, 200.0f, 0.0f),
          m_TranslationT0(350.0f, 200.0f, 0.0f)
    {
        m_Renderer2D = std::make_unique<Renderer2D>();
    }

    TestTriangle::~TestTriangle()
    {
    }

    void TestTriangle::OnUpdate(float deltaTime)
    {
    }

    void TestTriangle::OnRender()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Renderer2D->BeginScene(m_Camera);

        m_Renderer2D->DrawQuad(
            glm::vec2(m_TranslationQ0.x, m_TranslationQ0.y),
            glm::vec2(100.0f, 100.0f),
            Vec4{1.0f, 0.0f, 0.0f, 1.0f});
        m_Renderer2D->DrawTriangle(
            glm::vec2(m_TranslationT0.x, m_TranslationT0.y),
            100.0f,
            Vec4{0.0f, 1.0f, 0.0f, 1.0f});
        m_Renderer2D->DrawQuad(
            glm::vec2(m_TranslationQ1.x, m_TranslationQ1.y),
            glm::vec2(100.0f, 100.0f),
            Vec4{0.0f, 0.0f, 1.0f, 1.0f});

        m_Renderer2D->EndScene();
    }

    void TestTriangle::OnImGuiRender()
    {
        ImGui::SliderFloat3("Quad 0 Translation", &m_TranslationQ0.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Triangle Translation", &m_TranslationT0.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Quad 1 Translation", &m_TranslationQ1.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}