#include "bat.h"
#include "game.h"

Bat::Bat(float startX, float startY) noexcept
    : mSpeed(1000.0f), mDirection(Direction::None) {
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

  if (mDirection == Direction::Left && leftX > 0.f) {
    dx = -mSpeed;
  } else if (mDirection == Direction::Right && rightX < screenW) {
    dx = +mSpeed;
  }
  mShape.move({dx * dt.asSeconds(), 0.f});
}
