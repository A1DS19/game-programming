#include "player.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>

Player::Player()
    : mTexture(), mSprite(mTexture), mPosition{0.f, 0.f}, mResolution{0.f, 0.f},
      mArena{}, mTileSize{0}, mUpPressed{false}, mDownPressed{false},
      mLeftPressed{false}, mRightPressed{false},
      mHealth{static_cast<int>(START_HEALTH)},
      mMaxHealth{static_cast<int>(START_HEALTH)}, mLastHit(sf::Time::Zero),
      mSpeed{START_SPEED} {
  const std::string relPath = "../assets/graphics/player.png";
  const auto absPath = std::filesystem::absolute(relPath);
  std::cout << "Loading texture from “" << relPath
            << "” → absolute: " << absPath << "\n";

  if (!mTexture.loadFromFile(relPath)) {
    std::cerr << "❌ Failed to load texture from “" << absPath << "”\n";
  } else {
    auto sz = mTexture.getSize();
    std::cout << "✅ Success: texture size = " << sz.x << "×" << sz.y << "\n";
  }

  // When initializing texture and sprite reset rectangle.
  mSprite.setTexture(mTexture, true);
  mSprite.setOrigin({25.f, 25.f});
}

void Player::Spawn(const sf::IntRect &arena, const sf::Vector2f &resolution,
                   int tileSize) noexcept {
  mArena = arena;
  mResolution = resolution;
  mTileSize = tileSize;

  mPosition.x = arena.position.x + arena.size.x / 2.f;
  mPosition.y = arena.position.y + arena.size.y / 2.f;
  mSprite.setPosition(mPosition);

  std::cout << "player position: " << mPosition.x << " " << mPosition.y
            << std::endl;
}

void Player::ResetPlayerStats() noexcept {
  mSpeed = START_SPEED;
  mHealth = static_cast<int>(START_HEALTH);
  mMaxHealth = static_cast<int>(START_HEALTH);
}

bool Player::Hit(const sf::Time &timeHit) noexcept {
  if (timeHit.asMilliseconds() - mLastHit.asMilliseconds() > 200) {
    mLastHit = timeHit;
    mHealth -= 10;
    return true;
  }
  return false;
}

void Player::Move(Movement movement) noexcept {
  switch (movement) {
  case Movement::Up:
    mUpPressed = true;
    break;
  case Movement::Down:
    mDownPressed = true;
    break;
  case Movement::Left:
    mLeftPressed = true;
    break;
  case Movement::Right:
    mRightPressed = true;
    break;
  }
}

void Player::Stop(Movement movement) noexcept {
  switch (movement) {
  case Movement::Up:
    mUpPressed = false;
    break;
  case Movement::Down:
    mDownPressed = false;
    break;
  case Movement::Left:
    mLeftPressed = false;
    break;
  case Movement::Right:
    mRightPressed = false;
    break;
  }
}

void Player::Update(float elapsedTime, const sf::Vector2i &mousePosition) {
  if (mUpPressed)
    mPosition.y -= mSpeed * elapsedTime;
  if (mDownPressed)
    mPosition.y += mSpeed * elapsedTime;
  if (mRightPressed)
    mPosition.x += mSpeed * elapsedTime;
  if (mLeftPressed)
    mPosition.x -= mSpeed * elapsedTime;

  // The position can not be larger than the clamped position
  // search std::clamp
  mPosition.x =
      std::clamp(mPosition.x, float(mArena.position.x + mTileSize),
                 float(mArena.position.x + mArena.size.x - mTileSize));
  mPosition.y =
      std::clamp(mPosition.y, float(mArena.position.y + mTileSize),
                 float(mArena.position.y + mArena.size.y - mTileSize));

  mSprite.setPosition(mPosition);

  float dx = mousePosition.x - (mResolution.x * 0.5f);
  float dy = mousePosition.y - (mResolution.y * 0.5f);
  float angle = std::atan2(dy, dx);
  mSprite.setRotation(sf::radians(angle));
}

void Player::UpgradeSpeed() noexcept { mSpeed += (START_SPEED * 0.2f); }

void Player::UpgradeMaxHealth() noexcept {
  mMaxHealth += static_cast<int>(START_HEALTH * 0.2f);
}

void Player::IncreaseHealthLevel(unsigned int amount) noexcept {
  mHealth += amount;
  if (mHealth > mMaxHealth) {
    mHealth = mMaxHealth;
  }
}

std::ostream &operator<<(std::ostream &os, const Player &p) {
  auto pos = p.GetSprite().getPosition();
  os << "Player sprite { pos=(" << pos.x << ", " << pos.y << ") }";
  return os;
}
