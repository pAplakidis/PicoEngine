#pragma once

namespace PicoEngine
{
  template <typename T, typename... Args>
  T &Entity::AddComponent(Args &&...args)
  {
    return m_Scene->m_Registry.emplace<T>(
        m_EntityHandle,
        std::forward<Args>(args)...);
  }

  template <typename T>
  T &Entity::GetComponent()
  {
    return m_Scene->m_Registry.get<T>(m_EntityHandle);
  }

  template <typename T>
  bool Entity::HasComponent() const
  {
    return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
  }

  template <typename T>
  void Entity::RemoveComponent()
  {
    m_Scene->m_Registry.remove<T>(m_EntityHandle);
  }
}
