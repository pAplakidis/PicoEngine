// PrimitiveFactory.h

#pragma once

#include <array>

#include "BatchVertex.h"

namespace PrimitiveFactory
{
    std::array<Vertex, 3> CreateTriangle(float x, float y, float size, const Vec4 &color, float textureID = -1.0f);
    std::array<Vertex, 4> CreateQuad(float x, float y, float width, float height, const Vec4 &color, float textureID = -1.0f);

}
