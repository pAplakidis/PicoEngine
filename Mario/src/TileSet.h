#pragma once

#include <memory>
#include <unordered_map>

#include "Core/src/Renderer/GL/SubTexture2D.h"

// TODO: move to engine

enum class TileCollisionType
{
  None,
  Solid,
  OneWay
};

struct TileDefinition
{
  std::shared_ptr<PicoEngine::SubTexture2D> texture;
  TileCollisionType collisionType = TileCollisionType::None;
};

enum class TileType
{
  Empty,

  // Ground
  GroundTop,   // solid collider
  GroundFill,  // sollid collider
  GroundLeft,  // solid collider
  GroundRight, // solid collider

  // Blocks
  Brick,         // solid collider
  QuestionBlock, // solid collider
  UsedBlock,     // solid collider
  SolidBlock,    // solid collider
  HiddenBlock,   // solid collider

  // Pipes
  PipeTopLeft,
  PipeTopRight,
  PipeBodyLeft,
  PipeBodyRight,

  // Stairs / platforms
  StairBlock, // solid collider
  Platform,   // OneWay collider

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
