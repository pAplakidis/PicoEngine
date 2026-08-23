#include "IndexBuffer.h"

#include <GL/glew.h>
#include "GLCore/OpenGLDebug.h"

IndexBuffer::IndexBuffer(unsigned int maxCount)
    : m_Count(0)
{
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(
      GL_ELEMENT_ARRAY_BUFFER,
      m_RendererID));

  GLCall(glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      maxCount * sizeof(unsigned int),
      nullptr,
      GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::SetData(const unsigned int *data, unsigned int count)
{
  Bind();
  GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
  m_Count = count;
}
