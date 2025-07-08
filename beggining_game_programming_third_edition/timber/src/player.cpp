#include "player.hpp"
#include <iostream>

Player::Player(const std::string &texturePath, const sf::Vector2f &position)
    : texture(std::make_unique<sf::Texture>()), side(Game::Side::LEFT) {
  if (!texture->loadFromFile(texturePath)) {
    std::cerr << "could not load texture" << std::endl;
  }

  sprite = std::make_unique<sf::Sprite>(*texture);
  sprite->setPosition(position);
}
