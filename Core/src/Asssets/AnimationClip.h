#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Renderer/GL/SubTexture2D.h"

namespace PicoEngine
{
  struct AnimationFrame
  {
    std::shared_ptr<SubTexture2D> Frame;
    float Duration = 0.12f;
  };

  class AnimationClip
  {
  public:
    AnimationClip() = default;
    AnimationClip(const std::string &name, bool loop)
        : m_Name(name), m_IsLooping(loop)
    {
    }

    const std::string &GetName() const { return m_Name; }
    bool GetIsLooping() const { return m_IsLooping; }
    float GetDuration() const;

    void AddFrame(const std::shared_ptr<SubTexture2D> &frame, float duration = 0.12f);
    const AnimationFrame &GetFrame(size_t index) const;

    size_t GetFrameCount() const;

  private:
    std::string m_Name;
    bool m_IsLooping = false;
    std::vector<AnimationFrame> m_Frames;
  };
}
