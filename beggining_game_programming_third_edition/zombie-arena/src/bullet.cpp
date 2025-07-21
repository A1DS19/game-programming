#include "bullet.h"

Bullet::Bullet() noexcept : mInFlight(false), mBulletSpeed(1000.f) {
  mBulletShape.setSize({2, 2});
}

void Bullet::Shoot(float startX, float startY, float targetX,
                   float targetY) noexcept {
  // 1. Flag the bullet as active so the game loop will update / draw it.
  mInFlight = true;

  // 2. Put the bullet exactly where the shot originates.
  mBulletShape.setPosition({startX, startY});

  // 3. Build a direction vector from shooter to target.
  sf::Vector2f direction{targetX - startX, targetY - startY};

  // 4. Normalise that vector (length == 1).
  //    If the shooter and target are the same point we avoid division by
  //    zero by keeping the direction as (0,0).  The bullet will stay put.
  float length =
      std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length > 0.f)
    direction /= length;

  // 5. Scale the unit vector by the desired bullet speed to get pixels/sec
  //    (or pixels per frame, depending on how you apply the velocity).
  mVelocity = direction * mBulletSpeed;

  // 6. Reset distance counter and set max allowed range.
  mDistanceTravelled = 0.f;
  mMaxDistance = 1000.f; // same 1000-pixel limit as before
}

void Bullet::Update(float elapsedTime) noexcept {
  if (!mInFlight)
    return;

  sf::Vector2f displacement = mVelocity * elapsedTime;
  mBulletShape.move(displacement);
  mDistanceTravelled += std::sqrt(displacement.x * displacement.x +
                                  displacement.y * displacement.y);

  // Out of range?
  if (mDistanceTravelled >= mMaxDistance)
    mInFlight = false;
}

void Bullet::Stop() noexcept { mInFlight = false; }
