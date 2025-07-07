#include "log.hpp"
#include <iostream>

Log::Log(const std::string &texturePath, const sf::Vector2f &position)
    : texture(std::make_unique<sf::Texture>()), active(false), speedX(1000),
      speedY(-1500) {
  if (!texture->loadFromFile(texturePath)) {
    std::cerr << "could not load texture" << std::endl;
  }

  sprite = std::make_unique<sf::Sprite>(*texture);
  sprite->setPosition(position);
}
