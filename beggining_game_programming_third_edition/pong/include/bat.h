#pragma once

#include "SFML/Graphics.hpp"
#include "game.h"

class Bat {
public:
  explicit Bat(float startX, float startY) noexcept;
  const sf::RectangleShape &GetShape() const noexcept { return mShape; }
  void Move(const Game::Direction &direction) noexcept {
    mDirection = direction;
  }
  void Stop() noexcept { mDirection = Game::Direction::None; }
  // Run every frame to move the bat, based of the time(delta-time)
  void Update(const sf::Time &dt) noexcept;

private:
  sf::RectangleShape mShape;
  float mSpeed;
  Game::Direction mDirection;
};
