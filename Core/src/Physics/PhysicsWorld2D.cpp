#include "PhysicsWorld2D.h"

#include <algorithm>

#include "Component/TransformComponent.h"
#include "Component/BoxCollider2DComponent.h"
#include "Component/RigidBody2DComponent.h"
#include "Physics/Collision2D.h"
#include "Physics/AABB.h"

#include "Util/Log.h"

namespace PicoEngine
{

  void PhysicsWorld2D::Update(entt::registry &registry, CollisionSystem2D &collisionSystem, float dt)
  {
    auto view = registry.view<TransformComponent, BoxCollider2DComponent, RigidBody2DComponent>();
    for (auto entity : view)
    {
      auto &rigidBody = view.get<RigidBody2DComponent>(entity);

      // Ignore bodies that aren't Dynamic.
      if (rigidBody.GetBodyType() != BodyType::Dynamic)
        continue;

      MoveBody(registry, collisionSystem, entity, dt);
    }
  }

  void PhysicsWorld2D::MoveBody(entt::registry &registry, CollisionSystem2D &collisionSystem, entt::entity entity, float dt)
  {
    auto &transform = registry.get<TransformComponent>(entity);
    glm::vec3 position = transform.GetPosition();

    auto &rigidBody = registry.get<RigidBody2DComponent>(entity);
    glm::vec2 velocity = rigidBody.GetVelocity();

    glm::vec2 remainingMovement = velocity * dt;

    // In 2D, two blocking axes are usually sufficient.
    // Allow an extra iteration for corner cases.
    constexpr int MaxIterations = 3;

    for (int iteration = 0; iteration < MaxIterations; ++iteration)
    {
      if (glm::dot(remainingMovement, remainingMovement) < 1e-12f)
        break;

      // Query against the body's current position.
      const SweepResult2D hit = collisionSystem.SweepStatic(registry, entity, remainingMovement);

      LOG_TRACE(
          "Physics: velocity=({}, {}), displacement=({}, {}), "
          "hit={}, time={}, normal=({}, {})",
          velocity.x, velocity.y,
          remainingMovement.x, remainingMovement.y,
          hit.Hit, hit.Time,
          hit.Normal.x, hit.Normal.y);

      if (!hit.Hit)
      {
        position.x += remainingMovement.x;
        position.y += remainingMovement.y;
        transform.SetPosition(position);
        break;
      }

      // Move up to the collision boundary.
      const glm::vec2 allowedMovement = remainingMovement * hit.Time;
      position.x += allowedMovement.x;
      position.y += allowedMovement.y;
      transform.SetPosition(position);

      // Calculate movement still remaining this step.
      remainingMovement *= (1.0f - hit.Time);

      // Remove the component pointing into the obstacle.
      const float movementIntoSurface = glm::dot(remainingMovement, hit.Normal);
      if (movementIntoSurface < 0.0f)
      {
        remainingMovement -= movementIntoSurface * hit.Normal;
      }

      // Cancel velocity into the obstacle as well.
      const float velocityIntoSurface = glm::dot(velocity, hit.Normal);
      if (velocityIntoSurface < 0.0f)
      {
        velocity -= velocityIntoSurface * hit.Normal;
      }
    }

    rigidBody.SetVelocity(velocity);
  }
}
