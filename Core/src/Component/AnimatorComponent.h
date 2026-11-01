#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "Asssets/AnimationClip.h"
#include "Renderer/GL/SubTexture2D.h"

namespace PicoEngine
{
  class AnimationSystem;

  class AnimatorComponent
  {
    friend class AnimationSystem;

  public:
    AnimatorComponent() = default;

    void AddAnimation(const std::string &name, std::shared_ptr<const AnimationClip> clip);

    AnimationFrame GetCurrentFrame() const;

    void Play(const std::string &name, bool restart = false);
    void Stop();
    void Pause();
    void Resume();

    void SetSpeed(float speed);
    void SetFlipX(bool flipped);

    bool IsPlaying() const { return m_IsPlaying; };
    bool IsFinished() const { return m_IsFinished; };

  private:
    std::unordered_map<std::string, std::shared_ptr<const AnimationClip>> m_Animations;

    std::string m_CurrentAnimation;

    size_t m_FrameIndex = 0;
    float m_ElapsedTime = 0.0f;
    float m_PlaybackSpeed = 1.0f;

    bool m_IsPlaying = false;
    bool m_IsFinished = false;
    bool m_IsFlippedX = false;
  };
}
