#include "VertexArray.h"

#include <GL/glew.h>

#include "GLCore/OpenGLDebug.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
  Bind();
  vb.Bind();
  const auto &elements = layout.GetElements();
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(
        i,
        element.count,
        element.type,
        element.normalized,
        layout.GetStride(),
        reinterpret_cast<const void *>(element.offset));
  }
}

void VertexArray::Bind() const
{
  glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
  glBindVertexArray(0);
}
