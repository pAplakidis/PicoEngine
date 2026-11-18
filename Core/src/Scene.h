#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererDebugSettings.h"

#include "Systems/AnimationSystem.h"
#include "Systems/CollisionSystem2D.h"
#include "Physics/PhysicsWorld2D.h"

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

    void RenderCollisionDebug(Renderer2D &renderer, const RendererDebugSettings &settings);

    AnimationSystem &GetAnimationSystem() { return m_AnimationSystem; }
    CollisionSystem2D &GetCollisionSystem() { return m_CollisionSystem; }
    PhysicsWorld2D GetPhysicsWorld() { return m_PhysicsWorld; }

  private:
    entt::registry m_Registry;
    AnimationSystem m_AnimationSystem;
    PhysicsWorld2D m_PhysicsWorld;
    CollisionSystem2D m_CollisionSystem;
    friend class Entity;
  };

}

#include "Entity.inl"
