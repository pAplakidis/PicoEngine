#pragma once

#include <string>
#include <entt/entt.hpp>

#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Camera/OrthographicCamera.h"

namespace PicoEngine
{

  // Entity storage
  // Creation/Destruction
  // Game/Runtime update
  // Physics update
  // Rendering traversal
  class Scene
  {
  public:
    Scene() = default;
    ~Scene() = default;

    Entity CreateEntity(const std::string &name = "Entity");
    void DestroyEntity(Entity entity);

    void OnUpdate(float deltaTime);
    void OnRender(OrthographicCamera &camera, Renderer2D &renderer2D);

  private:
    entt::registry m_Registry;

    friend class Entity;
  };

}

#include "Entity.inl"
