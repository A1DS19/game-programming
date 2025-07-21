#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
public:
  Bullet() noexcept;

  void Stop() noexcept;
  bool IsInFlight() const noexcept { return mInFlight; }

  void Shoot(float startX, float startY, float targetX, float targetY) noexcept;

  // Bounding box in world space
  sf::FloatRect GetPosition() const noexcept {
    return mBulletShape.getGlobalBounds();
  }

  // Read-only access to the drawable shape
  const sf::RectangleShape &GetShape() const noexcept { return mBulletShape; }

  void Update(float elapsedTime) noexcept;

private:
  sf::RectangleShape mBulletShape; // geometry + position
  bool mInFlight = false;          // alive flag
  float mBulletSpeed = 1000.f;     // pixels per second

  sf::Vector2f mVelocity;         // direction * speed
  float mDistanceTravelled = 0.f; // how far it has gone
  float mMaxDistance = 1000.f;    // max allowed range
};
