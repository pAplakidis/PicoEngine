#pragma once

#include "Core/src/Entity.h"
#include "Core/src/Util/Input.h"

class MarioScript
{
public:
  static void Setup(PicoEngine::Entity entity)
  {
    auto &transform = entity.GetComponent<PicoEngine::TransformComponent>();
    transform.SetPosition({0.0f, 2.0f, 0.0f});
  }

  static void Loop(PicoEngine::Entity entity, float deltaTime)
  {
    auto &transform = entity.GetComponent<PicoEngine::TransformComponent>();

    glm::vec3 position = transform.GetPosition();

    constexpr float speed = 3.0f;

    if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_W))
      position.y += speed * deltaTime;

    if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_S))
      position.y -= speed * deltaTime;

    if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_A))
      position.x -= speed * deltaTime;

    if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_D))
      position.x += speed * deltaTime;

    transform.SetPosition(position);
  }
};
