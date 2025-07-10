#pragma once

#include "sfml/Graphics.hpp"

class Ball {
public:
  enum class ReboundPosition { Sides, Bat, Top, Bottom };
  explicit Ball(const float startX, const float startY) noexcept;
  sf::RectangleShape &GetShape() noexcept { return mShape; }
  const float &GetSpeed() const noexcept { return mSpeed; }
  void Update(const sf::Time &dt) noexcept;
  void Rebound(const ReboundPosition &rp) noexcept;

private:
  sf::RectangleShape mShape;
  float mSpeed;
  float mDirectionX;
  float mDirectionY;
};
