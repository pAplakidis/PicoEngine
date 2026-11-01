#include "AnimationClip.h"

namespace PicoEngine
{
  void AnimationClip::AddFrame(const std::shared_ptr<SubTexture2D> &frame, float duration)
  {
    m_Frames.push_back({frame, duration});
  }

  const AnimationFrame &AnimationClip::GetFrame(size_t index) const
  {
    return m_Frames[index];
  }

  size_t AnimationClip::GetFrameCount() const
  {
    return m_Frames.size();
  }

  float AnimationClip::GetDuration() const
  {
    float totalDuration = 0.0f;
    for (const auto &frame : m_Frames)
    {
      totalDuration += frame.Duration;
    }
    return totalDuration;
  }
}
