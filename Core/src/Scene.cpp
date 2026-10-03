#include "Scene.h"

#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/SpriteRendererComponent.h"

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

  void Scene::OnUpdate(float deltaTime) {}

  void Scene::OnRender(OrthographicCamera &camera, Renderer2D &renderer2D)
  {
    renderer2D.BeginScene(camera);

    for (auto entity : m_Registry.view<TransformComponent, SpriteRendererComponent>())
    {
      auto [transform, sprite] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);
      renderer2D.DrawQuad(transform.GetPosition(), transform.GetRotation(), transform.GetScale(), *sprite.GetTexture());
    }

    renderer2D.EndScene();
  }
}
