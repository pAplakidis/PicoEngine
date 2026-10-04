#pragma once

struct Vec2
{
  float x;
  float y;

  constexpr Vec2(float x, float y)
      : x(x), y(y)
  {
  }
};

struct Vec3
{
  float x, y, z;

  constexpr Vec3(float x, float y, float z)
      : x(x), y(y), z(z)
  {
  }
};

struct Vec4
{
  float x, y, z, w;

  constexpr Vec4(float x, float y, float z, float w)
      : x(x), y(y), z(z), w(w)
  {
  }
};

struct Vertex
{
  Vec3 Position;
  Vec4 Color;
  Vec2 TexCoords;
  float TexID;
};
