#include "MarioScript.h"

#include "Core/src/Scene.h"
#include "Core/src/Component/AnimatorComponent.h"
#include "Core/src/Component/SpriteRendererComponent.h"

void MarioScript::Setup(PicoEngine::Entity entity)
{
}

void MarioScript::Loop(PicoEngine::Entity entity, float deltaTime)
{
  auto &transform = entity.GetComponent<PicoEngine::TransformComponent>();
  auto &animator = entity.GetComponent<PicoEngine::AnimatorComponent>();

  float horizontal = 0.0f;

  if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_A))
    horizontal -= 1.0f;

  if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_D))
    horizontal += 1.0f;

  glm::vec3 position = transform.GetPosition();
  position.x += horizontal * MovementSpeed * deltaTime;
  transform.SetPosition(position);

  if (horizontal != 0.0f)
  {
    animator.Play("Walking");
    animator.SetFlipX(horizontal < 0.0f);
  }
  else
  {
    animator.Play("Idle");
  }
}
