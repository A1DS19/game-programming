#pragma once

#include "SFML/Graphics.hpp"
#include <memory>

struct Log {
  Log(const std::string &texturePath, const sf::Vector2f &position);

  bool active;
  float speedX;
  float speedY;
  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
};
