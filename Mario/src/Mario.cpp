#include "Application.h"

#include <imgui/imgui.h>

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
    m_ItemsSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/items-objects.png");
    m_CharactersSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/characters.png");
    m_MarioSubTexture = PicoEngine::SubTexture2D::CreateFromCoords(m_CharactersSpriteSheet, {16, 0}, {16.12, 33}, {1, 1});
  }

  void OnAttach() override
  {
    PicoEngine::Entity mario = m_Scene.CreateEntity("Mario");

    mario.AddComponent<PicoEngine::SpriteRendererComponent>(m_MarioSubTexture);
    // auto marioTexture = Texture(MARIO_RESOURCES_PATH "textures/Mario.png");
    // mario.AddComponent<PicoEngine::SpriteRendererComponent>(marioTexture);

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

  // FIXME: segfaults due to ImGui not being initialized yet, but this is the correct way to do it
  // TODO: implement Renderer2D stats
  // void OnImGuiRender() override
  // {
  //   ImGui::Begin("Renderer2D Stats"); // TODO: this should be in the ImGuiLayer, not here
  //   ImGui::Text("Draw calls: %u", PicoEngine::Renderer2D::GetDrawCallCount());
  //   ImGui::Text("Quads submitted: %u", PicoEngine::Renderer2D::GetQuadCount());
  //   ImGui::Text("Vertices submitted: %u", PicoEngine::Renderer2D::GetVertexCount());
  //   ImGui::Text("Indices submitted: %u", PicoEngine::Renderer2D::GetIndexCount());
  //   ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  //   ImGui::End();
  // }

  void OnEvent(PicoEngine::Event &event) override
  {
    m_CameraController.OnEvent(event);
  }

private:
  PicoEngine::Renderer2D &m_Renderer2D;
  PicoEngine::Scene m_Scene;
  PicoEngine::OrthographicCameraController m_CameraController;

  // TODO: do they have to be shared_ptr? or can they be unique_ptr? or not pointers at all?
  // better to be on the stack since they are sheets
  std::shared_ptr<Texture> m_ItemsSpriteSheet;
  std::shared_ptr<Texture> m_CharactersSpriteSheet;
  std::shared_ptr<PicoEngine::SubTexture2D> m_MarioSubTexture;
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
