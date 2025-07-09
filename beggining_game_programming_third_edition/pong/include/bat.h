#pragma once

#include "SFML/Graphics.hpp"

class Bat {
public:
  enum class Direction { Left, Right, None };
  explicit Bat(float startX, float startY) noexcept;
  const sf::RectangleShape &GetShape() const noexcept { return mShape; }
  void Move(const Direction &direction) noexcept { mDirection = direction; }
  void Stop() noexcept { mDirection = Direction::None; }
  // Run every frame to move the bat, based of the time(delta-time)
  void Update(const sf::Time &dt) noexcept;

private:
  sf::RectangleShape mShape;
  float mSpeed;
  Direction mDirection;
};
