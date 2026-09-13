#pragma once

#include <memory>

#include "Test.h"
#include "Renderer/Renderer2D.h"
#include "glm/glm.hpp"
#include "Camera/OrthographicCameraController.h"
#include "Event/Event.h"

namespace test
{
  class TestTransformations : public Test
  {
  public:
    TestTransformations();
    ~TestTransformations();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnEvent(PicoEngine::Event &event) override;
    void OnImGuiRender() override;

  private:
    PicoEngine::Renderer2D &m_Renderer2D;
    std::unique_ptr<Texture> m_Texture;
    PicoEngine::OrthographicCameraController m_CameraController;

    glm::vec3 m_Translation;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
  };
}
