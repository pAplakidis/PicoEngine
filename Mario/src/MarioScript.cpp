#include "MarioScript.h"

#include "Core/src/Scene.h"
#include "Core/src/Component/AnimatorComponent.h"
#include "Core/src/Component/SpriteRendererComponent.h"
#include "Core/src/Component/RigidBody2DComponent.h"

void MarioScript::Setup(PicoEngine::Entity entity)
{
}

void MarioScript::Loop(PicoEngine::Entity entity, float deltaTime)
{
  // Handle Controls
  float horizontal = 0.0f;
  if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_A))
    horizontal -= 1.0f;
  if (PicoEngine::Input::IsKeyPressed(GLFW_KEY_D))
    horizontal += 1.0f;

  // Handle movement (using physics)
  auto &rigidBody = entity.GetComponent<PicoEngine::RigidBody2DComponent>();
  glm::vec2 velocity = rigidBody.GetVelocity();
  velocity.x = horizontal * MovementSpeed;
  rigidBody.SetVelocity(velocity);

  // Handle Animation
  auto &animator = entity.GetComponent<PicoEngine::AnimatorComponent>();
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
