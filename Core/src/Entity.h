#pragma once

#include <entt/entt.hpp>
#include <utility>

namespace PicoEngine
{
  class Scene;

  class Entity
  {
  public:
    Entity() = default;

    Entity(entt::entity handle, Scene *scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

    template <typename T, typename... Args>
    T &AddComponent(Args &&...args);

    template <typename T>
    T &GetComponent();

    template <typename T>
    bool HasComponent() const;

    template <typename T>
    void RemoveComponent();

    operator bool() const
    {
      return m_EntityHandle != entt::null;
    }

  private:
    entt::entity m_EntityHandle{entt::null};
    Scene *m_Scene = nullptr;
    // TODO:
    // TransformComponent
    // SpriteRendererComponent
    // Rigidbody2DComponent
    // BoxCollider2DComponent
  };

}
