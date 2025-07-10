#include "bat.h"

Bat::Bat(float startX, float startY) noexcept
    : mSpeed(1000.0f), mDirection(Game::Direction::None) {
  mShape.setPosition({startX, startY});
  mShape.setFillColor(sf::Color::White);
  mShape.setSize({70, 5});
}

void Bat::Update(const sf::Time &dt) noexcept {
  float dx = 0.f;
  auto bounds = mShape.getGlobalBounds();
  float leftX = bounds.position.x;
  float rightX = bounds.position.x + bounds.size.x;
  float screenW = Game::screenDims.x;

  if (mDirection == Game::Direction::Left && leftX > 0.f) {
    dx = -mSpeed;
  } else if (mDirection == Game::Direction::Right && rightX < screenW) {
    dx = +mSpeed;
  }
  mShape.move({dx * dt.asSeconds(), 0.f});
}
