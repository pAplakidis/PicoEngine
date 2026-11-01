#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Systems/AnimationSystem.h"

namespace PicoEngine
{

  // Entity storage
  // Creation/Destruction
  // Game/Runtime update
  // Physics update
  // Rendering traversal
  // TODO: scene should eventually own the TileMap
  class Scene
  {
  public:
    Scene() = default;
    ~Scene() = default;

    Entity CreateEntity(const std::string &name = "Entity");
    void DestroyEntity(Entity entity);

    void OnUpdate(float deltaTime);
    void Render(Renderer2D &renderer2D);

  private:
    entt::registry m_Registry;
    AnimationSystem m_AnimationSystem;

    friend class Entity;
  };

}

#include "Entity.inl"
