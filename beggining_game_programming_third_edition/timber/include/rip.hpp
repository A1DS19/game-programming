
#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include <string>

struct Rip {
  Rip(const std::string &texturePath, const sf::Vector2f &position);

  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
};
