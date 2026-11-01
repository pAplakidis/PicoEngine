#pragma once

#include <array>
#include <memory>
#include <cmath>

#include <glm/glm.hpp>

#include "Core/src/Entity.h"
#include "Core/src/Util/Input.h"
#include "Core/src/Component/TransformComponent.h"

class MarioScript
{
public:
  explicit MarioScript()
  {
  }

  void Setup(PicoEngine::Entity entity);
  void Loop(PicoEngine::Entity entity, float deltaTime);

private:
  static constexpr float MovementSpeed = 3.0f;
};
