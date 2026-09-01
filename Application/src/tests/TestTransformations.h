#pragma once

#include <memory>

#include "Test.h"
#include "Renderer/Renderer2D.h"
#include "glm/glm.hpp"
#include "Camera/OrthographicCamera.h"

namespace test
{
  class TestTransformations : public Test
  {
  public:
    TestTransformations();
    ~TestTransformations();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    std::unique_ptr<Renderer2D> m_Renderer2D;
    std::unique_ptr<Texture> m_Texture;
    PicoEngine::OrthographicCamera m_Camera;

    glm::vec3 m_Translation;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
  };
}
