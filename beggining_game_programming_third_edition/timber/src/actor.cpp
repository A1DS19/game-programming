#include "actor.hpp"
#include <iostream>
#include <memory>

Actor::Actor(const std::string &textureFilePath, const sf::Vector2f &position)
    : active(false), speed(0.0f), texture(std::make_unique<sf::Texture>()) {
  if (!texture->loadFromFile(textureFilePath)) {
    std::cerr << "could not load texture" << std::endl;
  }

  sprite = std::make_unique<sf::Sprite>(*texture);
  sprite->setPosition(position);
}

void Actor::Initialize(const int topSpeed, const int speedOverflow,
                       const int maxHeight, const int heigthOverflow,
                       float initialPosition) {
  srand((int)time(0)); // seed rand
  speed = (rand() % topSpeed) + speedOverflow;

  // How high is the actor
  srand((int)time(0) * 10);
  height = (rand() % maxHeight) + heigthOverflow;
  sprite->setPosition({initialPosition, height});
  active = true;
}

void Actor::Initialize(const int topSpeed, const int maxHeight,
                       const int heigthOverflow, float initialPosition) {
  srand((int)time(0) * 10);
  speed = (rand() % topSpeed);

  srand((int)time(0) * 20);
  height = (rand() % maxHeight) - heigthOverflow;
  sprite->setPosition({initialPosition, height});
  active = true;
}

void Actor::Move(sf::Time &dt, const int restartOn,
                 const Direction &direction) {
  auto x = 0.0f;
  if (direction == Direction::ELeft) {
    x = sprite->getPosition().x - (speed * dt.asSeconds());
  } else {
    x = sprite->getPosition().x + (speed * dt.asSeconds());
  }

  auto y = sprite->getPosition().y;
  sprite->setPosition({x, y});

  x = sprite->getPosition().x;
  if (restartOn < 0 ? (x < restartOn) : (x > restartOn)) {
    active = false;
  }
}
