#pragma once

class IndexBuffer
{
public:
  IndexBuffer(unsigned int maxCount);
  ~IndexBuffer();

  void SetData(const unsigned int *data, unsigned int count);
  void Bind() const;
  void Unbind() const;

  unsigned int GetCount() const
  {
    return m_Count;
  }

private:
  unsigned int m_RendererID;
  unsigned int m_Count = 0;
};