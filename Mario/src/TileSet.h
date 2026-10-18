#pragma once

#include <memory>
#include <unordered_map>

#include "Core/src/Renderer/GL/SubTexture2D.h"

enum class TileType
{
  Empty,

  // Ground
  GroundTop,
  GroundFill,
  GroundLeft,
  GroundRight,

  // Blocks
  Brick,
  QuestionBlock,
  UsedBlock,
  SolidBlock,
  HiddenBlock,

  // Pipes
  PipeTopLeft,
  PipeTopRight,
  PipeBodyLeft,
  PipeBodyRight,

  // Stairs / platforms
  StairBlock,
  Platform,

  // Decorations / level structures
  FlagPole,
  FlagTop,
  CastleBlock,

  // Optional environment
  Cloud,
  Bush,
  Hill
};

class TileSet
{
public:
  TileSet() = default;

  void AddTile(TileType type, std::shared_ptr<PicoEngine::SubTexture2D> texture)
  {
    m_Tiles[type] = texture;
  }

  std::shared_ptr<PicoEngine::SubTexture2D> GetTile(TileType type) const
  {
    return m_Tiles.at(type);
  }

private:
  std::unordered_map<TileType, std::shared_ptr<PicoEngine::SubTexture2D>> m_Tiles;
};
