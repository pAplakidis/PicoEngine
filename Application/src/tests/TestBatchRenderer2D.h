#pragma once

#include <memory>

#include "Test.h"

#include "glm/glm.hpp"

#include "Renderer/Renderer2D.h"
#include "Texture.h"
#include "Camera/OrthographicCameraController.h"

namespace test
{
  class TestBatchRenderer2D : public Test
  {
  public:
    TestBatchRenderer2D();
    ~TestBatchRenderer2D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    std::unique_ptr<Renderer2D> m_Renderer2D;
    PicoEngine::OrthographicCameraController m_CameraController;

    std::unique_ptr<Texture> m_MarioTexture;
    std::unique_ptr<Texture> m_GoldDollarTexture;
  };
}
