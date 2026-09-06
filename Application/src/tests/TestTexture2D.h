#pragma once

#include <memory>

#include "Test.h"

#include "glm/glm.hpp"

#include "Renderer/Renderer2D.h"
#include "Renderer/GL/Texture.h"
#include "Camera/OrthographicCameraController.h"

namespace test
{
  class TestTexture2D : public Test
  {
  public:
    TestTexture2D();
    ~TestTexture2D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    PicoEngine::Renderer2D &m_Renderer2D;
    std::unique_ptr<Texture> m_Texture;
    PicoEngine::OrthographicCameraController m_CameraController;

    glm::vec3 m_TranslationA;
    glm::vec3 m_TranslationB;
  };
}
