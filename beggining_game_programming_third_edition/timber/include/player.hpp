#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include <string>

struct Player {
  enum class Side { LEFT, RIGHT, NONE };
  Player(const std::string &texturePath, const sf::Vector2f &position);

  Side side;
  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
};
