#include "AnimatorComponent.h"

namespace PicoEngine
{
  void AnimatorComponent::AddAnimation(const std::string &name, std::shared_ptr<const AnimationClip> clip)
  {
    m_Animations[name] = std::move(clip);
  }

  AnimationFrame AnimatorComponent::GetCurrentFrame() const
  {
    if (m_CurrentAnimation.empty())
      return {nullptr, 0.0f};

    const auto &clip = m_Animations.at(m_CurrentAnimation);
    return clip->GetFrame(m_FrameIndex);
  }

  void AnimatorComponent::Play(const std::string &name, bool restart)
  {
    if (m_CurrentAnimation == name && !restart)
      return;

    if (!m_Animations.contains(name))
      throw std::invalid_argument("Unknown animation: " + name);

    m_CurrentAnimation = name;
    m_FrameIndex = 0;
    m_ElapsedTime = 0.0f;
    m_IsPlaying = true;
    m_IsFinished = false;
  }

  void AnimatorComponent::Stop()
  {
    m_IsPlaying = false;
    m_IsFinished = true;
    m_FrameIndex = 0;
    m_ElapsedTime = 0.0f;
  }

  void AnimatorComponent::Pause()
  {
    m_IsPlaying = false;
  }

  void AnimatorComponent::Resume()
  {
    m_IsPlaying = true;
  }

  void AnimatorComponent::SetSpeed(float speed)
  {
    m_PlaybackSpeed = speed;
  }

  void AnimatorComponent::SetFlipX(bool flipped)
  {
    m_IsFlippedX = flipped;
  }
}
