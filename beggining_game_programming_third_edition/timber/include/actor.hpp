#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include <string>

struct Actor {
  enum class Direction { ELeft, ERigth };
  Actor(const std::string &textureFilePath, const sf::Vector2f &position);
  void Initialize(const int topSpeed, const int speedOverflow,
                  const int maxHeight, const int heigthOverflow,
                  float initialPosition);
  void Initialize(const int topSpeed, const int maxHeight,
                  const int heigthOverflow, float initialPosition);
  void Move(sf::Time &dt, const int restartOn, const Direction &direction);

  bool active;
  float speed;
  float height;
  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
};
