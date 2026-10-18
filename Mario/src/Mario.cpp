#include "Application.h"

#include <imgui/imgui.h>

#include "Core/src/Scene.h"
#include "Core/src/Entity.h"
#include "Core/src/Event/Event.h"
#include "Core/src/Renderer/RendererCommand.h"
#include "Core/src/Camera/OrthographicCameraController.h"
#include "Core/src/Layer.h"
#include "Core/src/Util/Log.h"

#include "Core/src/Component/SpriteRendererComponent.h"
#include "Core/src/Component/TransformComponent.h"

#include "Core/src/Renderer/Renderer.h"
#include "Core/src/Renderer/Renderer2D.h"
#include "Core/src/Renderer/RendererCommand.h"
#include "Core/src/Renderer/GL/Texture.h"

#include "TileSet.h"
#include "TileMap.h"

// TODO: add player and enemies spawn here
static constexpr const char *Level1Map =
    "................................................................\n"
    "................................................................\n"
    "...................?............................................\n"
    ".............B?B?B..............................................\n"
    ".............................................??.................\n"
    "............................LR..............B??B................\n"
    "............................lr..................................\n"
    "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n"
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n";

class GameLayer : public PicoEngine::Layer
{
public:
  GameLayer()
      : Layer("GameLayer"),
        m_Renderer2D(PicoEngine::Renderer::GetRenderer2D()),
        m_CameraController(
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetWidth()) /
            static_cast<float>(PicoEngine::Application::Get().GetWindow().GetHeight())),
        m_TileMap(Level1Map)
  {
    m_WorldSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/world_tileset.png");
    m_ItemsSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/items-objects.png");
    m_CharactersSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/characters.png");
    m_MarioSubTexture = PicoEngine::SubTexture2D::CreateFromCoords(m_CharactersSpriteSheet, {16, 0}, {16.12, 33}, {1, 1});

    // init tiles
    constexpr glm::vec2 TileSize{16.0f, 16.0f};

    m_TileSet.AddTile(TileType::GroundTop, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {0, 0}, TileSize));
    m_TileSet.AddTile(TileType::GroundFill, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {0, 1}, TileSize));
    m_TileSet.AddTile(TileType::GroundLeft, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {1, 0}, TileSize));
    m_TileSet.AddTile(TileType::GroundRight, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {1, 1}, TileSize));

    m_TileSet.AddTile(TileType::Brick, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {1, 0}, TileSize));
    m_TileSet.AddTile(TileType::QuestionBlock, PicoEngine::SubTexture2D::CreateFromCoords(m_ItemsSpriteSheet, {0, 4}, TileSize));
    m_TileSet.AddTile(TileType::UsedBlock, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {2, 2}, TileSize));
    m_TileSet.AddTile(TileType::SolidBlock, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {2, 0}, TileSize));
    m_TileSet.AddTile(TileType::HiddenBlock, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {4, 3}, TileSize));

    m_TileSet.AddTile(TileType::PipeTopLeft, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {0, 3}, TileSize));
    m_TileSet.AddTile(TileType::PipeTopRight, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {1, 3}, TileSize));
    m_TileSet.AddTile(TileType::PipeBodyLeft, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {0, 4}, TileSize));
    m_TileSet.AddTile(TileType::PipeBodyRight, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {1, 4}, TileSize));

    m_TileSet.AddTile(TileType::StairBlock, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {3, 3}, TileSize));
    m_TileSet.AddTile(TileType::Platform, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {10, 3}, TileSize));

    m_TileSet.AddTile(TileType::FlagPole, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {10, 3}, TileSize));
    m_TileSet.AddTile(TileType::FlagTop, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {9, 3}, TileSize));
    m_TileSet.AddTile(TileType::CastleBlock, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {3, 6}, TileSize));

    m_TileSet.AddTile(TileType::Cloud, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {2, 7}, TileSize));
    m_TileSet.AddTile(TileType::Bush, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {1, 6}, TileSize));
    m_TileSet.AddTile(TileType::Hill, PicoEngine::SubTexture2D::CreateFromCoords(m_WorldSpriteSheet, {3, 5}, TileSize));
  }

  void OnAttach() override
  {
    // Render player
    PicoEngine::Entity mario = m_Scene.CreateEntity("Mario");
    mario.AddComponent<PicoEngine::SpriteRendererComponent>(m_MarioSubTexture);
    auto &transform = mario.GetComponent<PicoEngine::TransformComponent>();
    transform.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    transform.SetRotation(0.0f);

    // Create map (TODO: add tilemap to Scene init, not game init)

    // Create enemies
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
    m_Renderer2D.BeginScene(m_CameraController.GetCamera());

    PicoEngine::RendererCommand::SetClearColor(0.36f, 0.67f, 1.0f, 1.0f);
    PicoEngine::RendererCommand::Clear();

    m_TileMap.Render(m_Renderer2D, m_TileSet);
    m_Scene.Render(m_Renderer2D);

    m_Renderer2D.EndScene();
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
  std::shared_ptr<Texture> m_WorldSpriteSheet;
  std::shared_ptr<Texture> m_ItemsSpriteSheet;
  std::shared_ptr<Texture> m_CharactersSpriteSheet;
  std::shared_ptr<PicoEngine::SubTexture2D> m_MarioSubTexture;

  TileSet m_TileSet;
  TileMap m_TileMap;
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
