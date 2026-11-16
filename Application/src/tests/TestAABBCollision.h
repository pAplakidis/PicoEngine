
#pragma once

#include "Test.h"

#include "Physics/AABB.h"
#include "Physics/Collision2D.h"
#include "Renderer/Renderer2D.h"
#include "Camera/OrthographicCameraController.h"

namespace test
{
  class TestAABBCollision : public Test
  {
  public:
    TestAABBCollision();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnEvent(PicoEngine::Event &event) override;
    void OnImGuiRender() override;

  private:
    void Reset();
    void DrawAABB(const PicoEngine::AABB &aabb, const Vec4 &color);

  private:
    PicoEngine::Renderer2D &m_Renderer2D;
    PicoEngine::OrthographicCameraController m_CameraController;

    glm::vec2 m_PositionA{-0.6f, 0.0f};
    glm::vec2 m_PositionB{0.6f, 0.0f};

    glm::vec2 m_SizeA{1.5f, 1.5f};
    glm::vec2 m_SizeB{1.5f, 1.5f};

    float m_Epsilon = 0.0001f;

    PicoEngine::CollisionManifold2D m_Manifold;
  };
}
