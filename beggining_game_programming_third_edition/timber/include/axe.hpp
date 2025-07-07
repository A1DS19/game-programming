#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include <string>

namespace Axes {
const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;
}; // namespace Axes

struct Axe {
  Axe(const std::string &texturePath, const sf::Vector2f &position);

  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
};
