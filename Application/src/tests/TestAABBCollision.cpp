
#include "TestAABBCollision.h"

#include <algorithm>

#include <GL/glew.h>
#include "imgui/imgui.h"

#include "Application.h"
#include "Renderer/Renderer.h"

namespace test
{
  TestAABBCollision::TestAABBCollision()
      : m_Renderer2D(PicoEngine::Renderer::GetRenderer2D()),
        m_CameraController(
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetWidth()) /
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetHeight()))
  {
    m_CameraController.SetZoomLevel(5.0f);
    m_CameraController.SetPanningEnabled(false);
    m_CameraController.SetZoomingEnabled(false);
  }

  void TestAABBCollision::Reset()
  {
    m_PositionA = {-0.6f, 0.0f};
    m_PositionB = {0.6f, 0.0f};

    m_SizeA = {1.5f, 1.5f};
    m_SizeB = {1.5f, 1.5f};

    m_Epsilon = 0.0001f;
  }

  void TestAABBCollision::OnUpdate(float deltaTime)
  {
    m_CameraController.OnUpdate(deltaTime);

    // Construct world-space AABBs from center and size.
    PicoEngine::AABB a{m_PositionA - m_SizeA * 0.5f, m_PositionA + m_SizeA * 0.5f};
    PicoEngine::AABB b{m_PositionB - m_SizeB * 0.5f, m_PositionB + m_SizeB * 0.5f};
    m_Manifold = PicoEngine::TestAABB(a, b, m_Epsilon);
  }

  void TestAABBCollision::DrawAABB(const PicoEngine::AABB &aabb, const Vec4 &color)
  {
    const glm::vec2 center = aabb.GetCenter();
    const glm::vec2 size = aabb.GetSize();

    // Draw rectangle outline using four thin quads.
    constexpr float thickness = 0.025f;

    m_Renderer2D.DrawQuad({center.x, aabb.max.y}, {size.x, thickness}, color);
    m_Renderer2D.DrawQuad({center.x, aabb.min.y}, {size.x, thickness}, color);
    m_Renderer2D.DrawQuad({aabb.min.x, center.y}, {thickness, size.y}, color);
    m_Renderer2D.DrawQuad({aabb.max.x, center.y}, {thickness, size.y}, color);
  }

  void TestAABBCollision::OnRender()
  {
    glClearColor(0.06f, 0.06f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    PicoEngine::AABB a{m_PositionA - m_SizeA * 0.5f, m_PositionA + m_SizeA * 0.5f};
    PicoEngine::AABB b{m_PositionB - m_SizeB * 0.5f, m_PositionB + m_SizeB * 0.5f};

    m_Renderer2D.BeginScene(m_CameraController.GetCamera());

    // Draw both box interiors.
    m_Renderer2D.DrawQuad(a.GetCenter(), a.GetSize(), Vec4{0.15f, 0.35f, 0.75f, 1.0f});
    m_Renderer2D.DrawQuad(b.GetCenter(), b.GetSize(), Vec4{0.75f, 0.4f, 0.15f, 1.0f});

    // Highlight the overlap area.
    if (m_Manifold.IsColliding)
    {
      glm::vec2 overlapMin = glm::max(a.min, b.min);
      glm::vec2 overlapMax = glm::min(a.max, b.max);

      glm::vec2 overlapSize = overlapMax - overlapMin;
      glm::vec2 overlapCenter = (overlapMin + overlapMax) * 0.5f;

      m_Renderer2D.DrawQuad(overlapCenter, overlapSize, Vec4{0.9f, 0.15f, 0.15f, 1.0f});
    }

    // Draw boundaries on top.
    DrawAABB(a, Vec4{0.3f, 0.6f, 1.0f, 1.0f});
    DrawAABB(b, Vec4{1.0f, 0.65f, 0.2f, 1.0f});

    m_Renderer2D.EndScene();
  }

  void TestAABBCollision::OnEvent(PicoEngine::Event &event)
  {
    m_CameraController.OnEvent(event);
  }

  void TestAABBCollision::OnImGuiRender()
  {
    ImGui::Text("AABB Collision Test");
    ImGui::Separator();

    ImGui::Text("Collider A");
    ImGui::SliderFloat2("Position A", &m_PositionA.x, -3.0f, 3.0f);
    ImGui::SliderFloat2("Size A", &m_SizeA.x, 0.1f, 3.0f);

    ImGui::Separator();

    ImGui::Text("Collider B");
    ImGui::SliderFloat2("Position B", &m_PositionB.x, -3.0f, 3.0f);
    ImGui::SliderFloat2("Size B", &m_SizeB.x, 0.1f, 3.0f);

    ImGui::Separator();

    ImGui::SliderFloat("Contact Epsilon", &m_Epsilon, 0.0f, 0.05f, "%.5f");

    ImGui::Separator();

    ImGui::TextColored(
        m_Manifold.IsColliding
            ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f)
            : ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
        "Colliding: %s",
        m_Manifold.IsColliding ? "YES" : "NO");

    ImGui::Text("Touching: %s", m_Manifold.IsTouching ? "YES" : "NO");

    ImGui::Text("Penetration: %.5f", m_Manifold.Penetration);

    ImGui::Text(
        "Normal: (%.2f, %.2f)",
        m_Manifold.Normal.x,
        m_Manifold.Normal.y);

    ImGui::Separator();

    if (ImGui::Button("Reset"))
      Reset();

    ImGui::SameLine();

    if (ImGui::Button("Separated"))
      m_PositionB = {2.5f, 0.0f};

    if (ImGui::Button("Exact Edge Contact"))
      m_PositionB = {
          m_PositionA.x + (m_SizeA.x + m_SizeB.x) * 0.5f,
          m_PositionA.y};

    if (ImGui::Button("Contained"))
    {
      m_PositionB = m_PositionA;
      m_SizeA = {2.5f, 2.5f};
      m_SizeB = {0.8f, 0.8f};
    }

    ImGui::Text("Frame time: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}
