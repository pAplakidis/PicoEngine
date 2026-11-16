#include "Scene.h"

#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/ScriptComponent.h"

namespace PicoEngine
{
  Entity Scene::CreateEntity(const std::string &name)
  {
    Entity entity{m_Registry.create(), this};
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
    return entity;
  }

  void Scene::DestroyEntity(Entity entity) {}

  void Scene::OnUpdate(float deltaTime)
  {
    auto view = m_Registry.view<ScriptComponent>();

    for (auto entityHandle : view)
    {
      Entity entity(entityHandle, this);

      // TODO: move to scripting system class
      // update scripting component
      auto &script = m_Registry.get<ScriptComponent>(entityHandle);
      if (!script.IsInitialized())
      {
        if (script.Setup)
        {
          script.Setup(entity);
        }

        script.Initialize();
      }
      if (script.Loop)
      {
        script.Loop(entity, deltaTime);
      }
    }

    // Update scene-wide systems once per frame
    m_AnimationSystem.Update(m_Registry, deltaTime);
    m_CollisionSystem.Update(m_Registry);
  }

  void Scene::Render(Renderer2D &renderer2D)
  {
    for (auto entity : m_Registry.view<TransformComponent, SpriteRendererComponent>())
    {
      auto [transform, sprite] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);
      if (sprite.HasSubTexture())
      {
        renderer2D.DrawQuad(transform.GetTransform(), *sprite.GetSubTexture(), sprite.GetFlipX(), sprite.GetFlipY());
      }
      else if (sprite.GetTexture())
      {
        renderer2D.DrawQuad(transform.GetTransform(), *sprite.GetTexture());
      }
    }
  }

  void Scene::RenderCollisionDebug(Renderer2D &renderer, const RendererDebugSettings &settings)
  {
    if (!settings.ShowColliders)
      return;

    auto view = m_Registry.view<TransformComponent, BoxCollider2DComponent>();
    const auto &contacts = m_CollisionSystem.GetContacts();
    for (auto entity : view)
    {
      const auto &transform = view.get<TransformComponent>(entity);
      const auto &collider = view.get<BoxCollider2DComponent>(entity);
      const AABB bounds = CalculateAABB(transform, collider);

      bool isColliding = false;
      bool isTouching = false;

      // Determine whether this collider has active contacts
      if (settings.ShowCollisionContacts)
      {
        for (const auto &contact : contacts)
        {
          if (contact.EntityA != entity && contact.EntityB != entity)
            continue;

          isColliding |= contact.Manifold.IsColliding;
          isTouching |= contact.Manifold.IsTouching;
        }
      }

      Vec4 color{0.2f, 0.9f, 0.4f, 1.0f};

      if (isColliding)
        color = Vec4{1.0f, 0.2f, 0.2f, 1.0f};
      else if (isTouching)
        color = Vec4{1.0f, 0.85f, 0.2f, 1.0f};

      renderer.DrawAABB(bounds, color);
    }
  }
}
