#pragma once

#include <memory>

#include "Test.h"
#include "Renderer/Renderer2D.h"
#include "glm/glm.hpp"
#include "Camera/OrthographicCamera.h"

namespace test
{
  class TestTriangle : public Test
  {
  public:
    TestTriangle();
    ~TestTriangle();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    std::unique_ptr<Renderer2D> m_Renderer2D;
    std::unique_ptr<PicoEngine::OrthographicCamera> m_Camera;

    glm::vec3 m_TranslationQ0;
    glm::vec3 m_TranslationQ1;
    glm::vec3 m_TranslationT0;
  };
}