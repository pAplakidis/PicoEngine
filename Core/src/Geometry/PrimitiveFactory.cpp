// PrimitiveFactory.cpp

#include "PrimitiveFactory.h"

#include <cmath>

namespace PrimitiveFactory
{

  std::array<Vertex, 4> CreateQuad(
      float x,
      float y,
      float width,
      float height,
      const Vec4 &color,
      float textureID)
  {
    return {
        Vertex{
            {x, y, 0.0f},
            color,
            {0.0f, 0.0f},
            textureID},

        Vertex{
            {x + width, y, 0.0f},
            color,
            {1.0f, 0.0f},
            textureID},

        Vertex{
            {x + width, y + height, 0.0f},
            color,
            {1.0f, 1.0f},
            textureID},

        Vertex{
            {x, y + height, 0.0f},
            color,
            {0.0f, 1.0f},
            textureID}};
  }

  std::array<Vertex, 3> CreateTriangle(
      float x,
      float y,
      float size,
      const Vec4 &color,
      float textureID)
  {
    const float height = size * std::sqrt(3.0f) / 2.0f;

    return {
        Vertex{
            {x, y, 0.0f},
            color,
            {0.5f, 0.0f},
            textureID},

        Vertex{
            {x + size, y, 0.0f},
            color,
            {1.0f, 0.0f},
            textureID},

        Vertex{
            {x + size / 2.0f, y + height, 0.0f},
            color,
            {0.5f, 1.0f},
            textureID}};
  }

}
