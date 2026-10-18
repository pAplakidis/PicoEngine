#pragma once

#include <vector>

#include "TileSet.h"
#include "Core/src/Renderer/Renderer2D.h"

using TileID = TileType;

class TileMap
{
public:
  // TODO: load from file
  TileMap(const std::string &map);

  TileType GetTile(unsigned int x, unsigned int y) const
  {
    if (x >= m_Width || y >= m_Height)
      return TileType::Empty;

    return m_Tiles[y * m_Width + x];
  }

  void Render(PicoEngine::Renderer2D &renderer, const TileSet &tileSet) const;

private:
  unsigned int m_Width = 0;
  unsigned int m_Height = 0;
  std::vector<TileID> m_Tiles; // TODO: this could just be a flat array
};
