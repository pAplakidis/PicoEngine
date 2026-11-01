#include "AnimationSystem.h"

#include <cmath>

#include "Component/AnimatorComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"

namespace PicoEngine
{
  void AnimationSystem::Update(entt::registry &registry, float deltaTime)
  {
    auto view = registry.view<AnimatorComponent, SpriteRendererComponent, TransformComponent>();

    for (auto entity : view)
    {
      auto &animator = view.get<AnimatorComponent>(entity);
      auto &sprite = view.get<SpriteRendererComponent>(entity);
      auto &transform = view.get<TransformComponent>(entity);

      // Find the current clip
      auto it = animator.m_Animations.find(animator.m_CurrentAnimation);
      if (it == animator.m_Animations.end() || !it->second)
        continue;

      const auto &clip = *it->second;
      const size_t frameCount = clip.GetFrameCount();
      if (frameCount == 0)
        continue;

      if (animator.m_FrameIndex >= frameCount)
        animator.m_FrameIndex = 0;

      // Advance animation playback
      if (animator.m_IsPlaying && animator.m_PlaybackSpeed > 0.0f && deltaTime > 0.0f)
      {
        animator.m_ElapsedTime += deltaTime * animator.m_PlaybackSpeed;

        while (true)
        {
          const auto &currentFrame = clip.GetFrame(animator.m_FrameIndex);
          const float duration = currentFrame.Duration;

          // Prevent infinite loops for invalid frame durations
          if (duration <= 0.0f)
            break;

          if (animator.m_ElapsedTime < duration)
            break;

          animator.m_ElapsedTime -= duration;

          if (animator.m_FrameIndex + 1 < frameCount)
          {
            animator.m_FrameIndex++;
          }
          else if (clip.GetIsLooping())
          {
            animator.m_FrameIndex = 0;
          }
          else
          {
            // Non-looping clip finished.
            animator.m_FrameIndex = frameCount - 1;
            animator.m_ElapsedTime = 0.0f;
            animator.m_IsPlaying = false;
            animator.m_IsFinished = true;
            break;
          }
        }
      }

      // Apply the active animation frame.
      const auto &frame = clip.GetFrame(animator.m_FrameIndex);

      if (sprite.GetSubTexture() != frame.Frame)
        sprite.SetSubTexture(frame.Frame);

      // Temporary mirroring through TransformComponent.
      glm::vec3 scale = transform.GetScale();
      const float absoluteX = std::abs(scale.x);

      scale.x = animator.m_IsFlippedX ? -absoluteX : absoluteX;
      transform.SetScale(scale);
    }
  }
}
