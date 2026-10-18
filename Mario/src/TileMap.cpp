#include "TileMap.h"

#include <sstream>
#include "glm/gtc/matrix_transform.hpp"

#include "Core/src/Util/Log.h"

TileType CharToTileType(char c)
{
  switch (c)
  {
  case '.':
    return TileType::Empty;

  // Ground
  case 'T':
    return TileType::GroundTop;
  case 'G':
    return TileType::GroundFill;
  case '[':
    return TileType::GroundLeft;
  case ']':
    return TileType::GroundRight;

  // Blocks
  case 'B':
    return TileType::Brick;
  case '?':
    return TileType::QuestionBlock;
  case 'U':
    return TileType::UsedBlock;
  case 'S':
    return TileType::SolidBlock;
  case 'H':
    return TileType::HiddenBlock;

  // Pipes
  case 'L':
    return TileType::PipeTopLeft;
  case 'R':
    return TileType::PipeTopRight;
  case 'l':
    return TileType::PipeBodyLeft;
  case 'r':
    return TileType::PipeBodyRight;

  // Stairs / platforms
  case '#':
    return TileType::StairBlock;
  case '=':
    return TileType::Platform;

  // Flag / castle
  case '|':
    return TileType::FlagPole;
  case 'F':
    return TileType::FlagTop;
  case 'C':
    return TileType::CastleBlock;

  // Decoration
  case 'c':
    return TileType::Cloud;
  case 'b':
    return TileType::Bush;
  case 'h':
    return TileType::Hill;

  default:
    return TileType::Empty;
  }
}

// TODO: move to Utils
std::string LoadTextFile(const std::string &filepath)
{
  std::ifstream file(filepath);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

TileMap::TileMap(const std::string &map)
{
  std::string line;
  std::istringstream stream(map);

  while (std::getline(stream, line))
  {
    if (m_Width == 0)
      m_Width = line.length();

    if (line.size() != m_Width)
    {
      LOG_CORE_ERROR("TileMap row has incorrect width");
      return;
    }

    for (char c : line)
    {
      m_Tiles.push_back(CharToTileType(c));
    }

    ++m_Height;
  }
}

void TileMap::Render(PicoEngine::Renderer2D &renderer, const TileSet &tileSet) const
{
  for (unsigned int y = 0; y < m_Height; y++)
  {
    for (unsigned int x = 0; x < m_Width; x++)
    {
      TileType tileType = GetTile(x, y);

      if (tileType == TileType::Empty)
        continue;

      auto tileTexture = tileSet.GetTile(tileType);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(x), static_cast<float>(m_Height - 1 - y), 0.0f));
      renderer.DrawQuad(transform, *tileTexture);
    }
  }
}
