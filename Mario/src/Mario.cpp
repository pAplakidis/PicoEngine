#include "Application.h"

#include "Core/src/Scene.h"
#include "Core/src/Entity.h"
#include "Core/src/Event/Event.h"
#include "Core/src/Camera/OrthographicCameraController.h"
#include "Core/src/Layer.h"
#include "Core/src/Util/Log.h"

#include "Core/src/Component/SpriteRendererComponent.h"
#include "Core/src/Component/TransformComponent.h"

#include "Core/src/Renderer/Renderer.h"
#include "Core/src/Renderer/Renderer2D.h"
#include "Core/src/Renderer/RendererCommand.h"
#include "Core/src/Renderer/GL/Texture.h"

class GameLayer : public PicoEngine::Layer
{
public:
  GameLayer()
      : Layer("GameLayer"),
        m_Renderer2D(PicoEngine::Renderer::GetRenderer2D()),
        m_CameraController(
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetWidth()) /
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetHeight()))
  {
  }

  void OnAttach() override
  {
    PicoEngine::Entity mario = m_Scene.CreateEntity("Mario");

    // TODO: load from texture atlas
    mario.AddComponent<PicoEngine::SpriteRendererComponent>(std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/mario.png"));

    auto &transform = mario.GetComponent<PicoEngine::TransformComponent>();
    transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    transform.SetRotation(0.0f);
    transform.SetScale(glm::vec3(0.5f, 0.5f, 1.0f));

    // Create map
    // Create enemies
    // Load sprite sheet and create animations
  }

  void OnDetach() override
  {
  }

  void OnUpdate(float deltaTime) override
  {
    m_CameraController.OnUpdate(deltaTime);

    // TODO: m_Scene.OnUpdate(deltaTime);
  }

  void OnRender() override
  {
    m_Scene.OnRender(m_CameraController.GetCamera(), m_Renderer2D);
  }

  void OnEvent(PicoEngine::Event &event) override
  {
    m_CameraController.OnEvent(event);
  }

private:
  PicoEngine::Renderer2D &m_Renderer2D;
  PicoEngine::Scene m_Scene;
  PicoEngine::OrthographicCameraController m_CameraController;
};

class MarioApplication : public PicoEngine::Application
{
public:
  MarioApplication()
  {
    PushLayer(new GameLayer());
  }
};

int main()
{
  PicoEngine::Log::Init();
  MarioApplication app;
  app.Run();
  return 0;
}
