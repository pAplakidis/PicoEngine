#include "Application.h"

#include <imgui/imgui.h>
#include <array>

#include "Core/src/Scene.h"
#include "Core/src/Entity.h"
#include "Core/src/Event/Event.h"
#include "Core/src/Renderer/RendererCommand.h"
#include "Core/src/Camera/OrthographicCameraController.h"
#include "Core/src/Layer.h"
#include "Core/src/Util/Log.h"

#include "Core/src/Component/SpriteRendererComponent.h"
#include "Core/src/Component/TransformComponent.h"
#include "Core/src/Component/ScriptComponent.h"
#include "Core/src/Component/AnimatorComponent.h"

#include "Core/src/Renderer/Renderer.h"
#include "Core/src/Renderer/Renderer2D.h"
#include "Core/src/Renderer/RendererCommand.h"
#include "Core/src/Renderer/GL/Texture.h"

#include "Core/src/Asssets/AnimationClip.h"

#include "TileSet.h"
#include "TileMap.h"
#include "MarioScript.h"

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
    // Load sprite sheets
    m_WorldSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/world_tileset.png");
    m_ItemsSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/items-objects.png");
    m_CharactersSpriteSheet = std::make_shared<Texture>(MARIO_RESOURCES_PATH "textures/mario_bros.png");

    // setup mario subtextures
    struct SpriteRect
    {
      float x, y, width, height;
    };

    // TODO: move to a json file or something, and load it at runtime
    // Small Mario
    constexpr std::array<SpriteRect, 11> SmallRects{{{178, 32, 12, 16}, // Standing
                                                     {80, 32, 15, 16},  // Walking 1
                                                     {96, 32, 16, 16},  // Walking 2
                                                     {112, 32, 16, 16}, // Walking 3
                                                     {144, 32, 16, 16}, // Jumping
                                                     {130, 32, 14, 16}, // Turning
                                                     {160, 32, 15, 16},
                                                     {320, 8, 16, 24},
                                                     {241, 33, 16, 16},
                                                     {194, 32, 12, 16},
                                                     {210, 33, 12, 16}}};

    // Big Mario
    constexpr std::array<SpriteRect, 11> BigRects{{{176, 0, 16, 32},
                                                   {81, 0, 16, 32},
                                                   {97, 0, 15, 32},
                                                   {113, 0, 15, 32},
                                                   {144, 0, 16, 32},
                                                   {128, 0, 16, 32},
                                                   {336, 0, 16, 32},
                                                   {160, 10, 16, 22},
                                                   {272, 2, 16, 29},
                                                   {193, 2, 16, 29},
                                                   {209, 2, 16, 29}}};

    // Fire Mario
    constexpr std::array<SpriteRect, 11> FireRects{{{176, 48, 16, 32},
                                                    {81, 48, 16, 32},
                                                    {97, 48, 15, 32},
                                                    {113, 48, 15, 32},
                                                    {144, 48, 16, 32},
                                                    {128, 48, 16, 32},
                                                    {336, 48, 16, 32},
                                                    {160, 58, 16, 22},
                                                    {272, 50, 16, 29},
                                                    {193, 50, 16, 29},
                                                    {209, 50, 16, 29}}};

    // Helper to create a subtexture from a pixel rectangle.
    const auto makeFrame = [this](const SpriteRect &r)
    {
      return PicoEngine::SubTexture2D::CreateFromPixels(m_CharactersSpriteSheet, {r.x, r.y}, {r.x + r.width, r.y + r.height});
    };

    // Load all Mario frames.
    for (size_t i = 0; i < SmallRects.size(); ++i)
    {
      m_MarioSmallFrames[i] = makeFrame(SmallRects[i]);
      m_MarioBigFrames[i] = makeFrame(BigRects[i]);
      m_MarioFireFrames[i] = makeFrame(FireRects[i]);
    }

    // TODO: add AnimationController that holds the map of named clips
    // Make animation clips for Mario
    m_IdleAnimationClip = std::make_shared<PicoEngine::AnimationClip>("Idle", true);
    m_IdleAnimationClip->AddFrame(m_MarioSmallFrames[0], 0.12f);

    m_WalkingAnimationClip = std::make_shared<PicoEngine::AnimationClip>("Walking", true);
    m_WalkingAnimationClip->AddFrame(m_MarioSmallFrames[1], 0.12f);
    m_WalkingAnimationClip->AddFrame(m_MarioSmallFrames[2], 0.12f);
    m_WalkingAnimationClip->AddFrame(m_MarioSmallFrames[3], 0.12f);

    m_JumpingAnimationClip = std::make_shared<PicoEngine::AnimationClip>("Jumping", false);
    m_JumpingAnimationClip->AddFrame(m_MarioSmallFrames[4], 0.12f);

    m_CameraController.SetZoomLevel(10.0f);

    // init world tiles
    // TODO: make collidable
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
    mario.AddComponent<PicoEngine::SpriteRendererComponent>(m_MarioSmallFrames[0]);

    auto &animator = mario.AddComponent<PicoEngine::AnimatorComponent>();
    animator.AddAnimation("Idle", m_IdleAnimationClip);
    animator.AddAnimation("Walking", m_WalkingAnimationClip);
    animator.AddAnimation("Jumping", m_JumpingAnimationClip);
    animator.Play("Idle");

    auto marioScript = std::make_shared<MarioScript>();
    mario.AddComponent<PicoEngine::ScriptComponent>(
        [marioScript](PicoEngine::Entity entity)
        {
          marioScript->Setup(entity);
        },
        [marioScript](PicoEngine::Entity entity, float deltaTime)
        {
          marioScript->Loop(entity, deltaTime);
        });

    // TODO:
    // give ScriptComponent a templated helper : auto &script = mario.AddComponent<ScriptComponent>();
    // script.Bind<MarioScript>();
    // Then MarioScript could simply be : class MarioScript
    // {
    // public:
    //   void Setup(PicoEngine::Entity entity)
    //   {
    //   }

    //   void Loop(PicoEngine::Entity entity, float dt)
    //   {
    //   }
    // };

    // Create map (TODO: add tilemap to Scene init, not game init)

    // Create enemies
  }

  void
  OnDetach() override
  {
  }

  void OnUpdate(float deltaTime) override
  {
    m_CameraController.OnUpdate(deltaTime);
    m_Scene.OnUpdate(deltaTime);
  }

  void OnRender() override
  {
    // Reset stats
    PicoEngine::Renderer2D::ResetStats();

    PicoEngine::RendererCommand::SetClearColor(0.36f, 0.67f, 1.0f, 1.0f);
    PicoEngine::RendererCommand::Clear();

    m_Renderer2D.BeginScene(m_CameraController.GetCamera());
    m_TileMap.Render(m_Renderer2D, m_TileSet); // TODO: this should be inside Scene
    m_Scene.Render(m_Renderer2D);
    m_Renderer2D.EndScene();

    // TODO: move to ImGui
    auto stats = PicoEngine::Renderer2D::GetStats();
    LOG_INFO("[Stats] DrawCalls {}", stats.DrawCalls);
  }

  // FIXME: segfaults due to ImGui not being initialized yet, but this is the correct way to do it
  // TODO: implement Renderer2D stats
  // void OnImGuiRender() override
  // {
  //   auto stats = PicoEngine::Renderer2D::GetStats();
  //   ImGui::Begin("Renderer2D Stats"); // TODO: this should be in the ImGuiLayer, not here
  //   ImGui::Text("Draw calls: %u", stats.DrawCalls);
  //   ImGui::Text("Quad Count: %u", stats.QuadCount );
  //   ImGui::Text("Vertices submitted: %u", stats.GetTotalVertexCount());
  //   ImGui::Text("Indices submitted: %u", stats.GetTotalIndexCount());
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

  // TODO: MarioBigFireFrames
  std::array<std::shared_ptr<PicoEngine::SubTexture2D>, 11> m_MarioSmallFrames;
  std::array<std::shared_ptr<PicoEngine::SubTexture2D>, 11> m_MarioBigFrames;
  std::array<std::shared_ptr<PicoEngine::SubTexture2D>, 11> m_MarioFireFrames;

  std::shared_ptr<PicoEngine::AnimationClip> m_IdleAnimationClip;
  std::shared_ptr<PicoEngine::AnimationClip> m_WalkingAnimationClip;
  std::shared_ptr<PicoEngine::AnimationClip> m_JumpingAnimationClip;

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
