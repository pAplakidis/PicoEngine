#pragma once

#include <memory>

#include "Test.h"

#include "glm/glm.hpp"

#include "Renderer/Renderer2D.h"
#include "Texture.h"
#include "Camera/OrthographicCamera.h"

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
    std::unique_ptr<Renderer2D> m_Renderer2D;
    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<PicoEngine::OrthographicCamera> m_Camera;

    glm::vec3 m_TranslationA;
    glm::vec3 m_TranslationB;
  };
}
