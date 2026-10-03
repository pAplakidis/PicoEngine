#include "Application.h"

#include "Core/src/Scene.h"
#include "Core/src/Entity.h"
#include "Core/src/Event/Event.h"
#include "Core/src/Camera/OrthographicCameraController.h"

#include "Core/src/Component/SpriteRendererComponent.h"
#include "Core/src/Component/TransformComponent.h"

#include "Core/src/Renderer/Renderer.h"
#include "Core/src/Renderer/Renderer2D.h"
#include "Core/src/Renderer/RendererCommand.h"
#include "Core/src/Renderer/GL/Texture.h"

#include "Core/src/Util/Log.h"

class MarioApplication : public PicoEngine::Application
{
public:
  MarioApplication()
      : m_Renderer2D(PicoEngine::Renderer::GetRenderer2D()),
        m_CameraController(
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetWidth()) /
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetHeight()))
  {
    // TODO: load from texture atlas
    PicoEngine::Entity mario = m_Scene.CreateEntity("Mario");
    mario.AddComponent<PicoEngine::SpriteRendererComponent>(std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/mario.png"));
    mario.GetComponent<PicoEngine::TransformComponent>().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    mario.GetComponent<PicoEngine::TransformComponent>().SetRotation(0.0f);
    mario.GetComponent<PicoEngine::TransformComponent>().SetScale(glm::vec3(0.5f, 0.5f, 1.0f));
  }

protected:
  void OnRender() override
  {
    PicoEngine::RendererCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    PicoEngine::RendererCommand::Clear();
    m_Scene.OnRender(m_CameraController.GetCamera(), m_Renderer2D);
  }

  void OnUpdate(float deltaTime) override
  {
    m_CameraController.OnUpdate(deltaTime);
  }

  void OnEvent(PicoEngine::Event &event)
  {
    m_CameraController.OnEvent(event);
  }

private:
  PicoEngine::Renderer2D &m_Renderer2D;
  PicoEngine::Scene m_Scene;
  PicoEngine::OrthographicCameraController m_CameraController;
};

int main()
{
  PicoEngine::Log::Init();
  MarioApplication app;
  app.Run();
  return 0;
}
