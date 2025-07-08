#pragma once

#include "SFML/Graphics.hpp"
#include "game.hpp"
#include <memory>
#include <string>

struct Player {
  Player(const std::string &texturePath, const sf::Vector2f &position);

  Game::Side side;
  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
};
