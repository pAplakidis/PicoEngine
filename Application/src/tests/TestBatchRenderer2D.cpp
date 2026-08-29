#include "TestBatchRenderer2D.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer/Renderer2D.h"
#include "GLCore/OpenGLDebug.h"

namespace test
{
    TestBatchRenderer2D::TestBatchRenderer2D()
        : m_Proj(glm::ortho(
              0.0f,
              960.0f,
              0.0f,
              540.0f,
              -1.0f,
              1.0f)),
          m_View(glm::mat4(1.0f))
    {
        m_Renderer2D =
            std::make_unique<Renderer2D>();

        m_MarioTexture =
            std::make_unique<Texture>(
                APPLICATION_RESOURCES_PATH
                "textures/mario.png");

        m_GoldDollarTexture =
            std::make_unique<Texture>(
                APPLICATION_RESOURCES_PATH
                "textures/gold-dollar.png");
    }

    TestBatchRenderer2D::~TestBatchRenderer2D()
    {
    }

    void TestBatchRenderer2D::OnUpdate(float deltaTime)
    {
    }

    void TestBatchRenderer2D::OnRender()
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Renderer2D->BeginScene(m_Proj, m_View);

        constexpr uint32_t Columns = 100;

        constexpr float QuadSize = 5.0f;
        constexpr float Gap = 0.25f;

        const uint32_t quadCount =
            Renderer2D::GetMaxQuads() + 8;

        for (uint32_t i = 0; i < quadCount; i++)
        {
            const uint32_t column = i % Columns;
            const uint32_t row = i / Columns;

            const float x = 20.0f + column * (QuadSize + Gap);

            const float y = 10.0f + row * (QuadSize + Gap);

            const bool useMario = (column + row) % 2 == 0;

            const Texture &texture = useMario ? *m_MarioTexture : *m_GoldDollarTexture;

            m_Renderer2D->DrawQuad({x, y}, {QuadSize, QuadSize}, texture);
        }

        m_Renderer2D->EndScene();
    }

    void TestBatchRenderer2D::OnImGuiRender()
    {
        ImGui::Text("Batch capacity: %u quads", Renderer2D::GetMaxQuads());
        ImGui::Text("Quads submitted: %u", Renderer2D::GetMaxQuads() + 8);
        ImGui::Text("Expected minimum draw calls: 2");
        ImGui::Text("Textures: mario.png + gold-dollar.png");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
