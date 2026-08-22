#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GLCore/OpenGLDebug.h"

class Renderer
{
public:
  void Clear() const;
  void Draw(VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
