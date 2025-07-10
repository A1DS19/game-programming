#include "ball.h"
#include "game.h"

Ball::Ball(const float startX, const float startY) noexcept
    : mSpeed(300.0f), mDirectionX(1.f), mDirectionY(1.f) {
  mShape.setSize({10, 10});
  mShape.setFillColor(sf::Color::White);
  mShape.setPosition({startX, startY});
}

void Ball::Update(const sf::Time &dt) noexcept {
  float dtSec = dt.asSeconds();
  // directionX is ±1 (or whatever), speed is pixels/sec
  sf::Vector2f movement{mDirectionX * mSpeed * dtSec,
                        mDirectionY * mSpeed * dtSec};
  mShape.move(movement);
}

void Ball::Rebound(const Ball::ReboundPosition &rp) noexcept {
  switch (rp) {
  case Ball::ReboundPosition::Top:
    mDirectionY = std::abs(mDirectionY);
    break;
  case Ball::ReboundPosition::Sides:
    mDirectionX = -mDirectionX;
    break;
  case Ball::ReboundPosition::Bottom:
    mShape.setPosition({Game::screenDims.x / 2.f, Game::screenDims.y / 2.f});
    mDirectionY = -std::abs(mDirectionY);
    break;
  case Ball::ReboundPosition::Bat:
    mDirectionY = -std::abs(mDirectionY);
    break;
  }
}
