#include "zombie.h"
#include "textureHolder.h"
#include <cstdlib>
#include <ctime>

Zombie::Zombie() noexcept
    : mPosition(0.f, 0.f), mSpeed(0.f), mHealth(0.f), mAlive(false), mTexture(),
      mSprite(mTexture) {}

bool Zombie::Hit() noexcept {
  mHealth--;
  if (mHealth < 0) {
    mAlive = false;
    mSprite.setTexture(
        TextureHolder::GetTexture("../assets/graphics/blood.png"));
    return true;
  }

  // Injured not dead
  return false;
}

const Zombie::Stats Zombie::GetZombieStat(Type type) noexcept {
  auto zombie = zombieStats.find(type);
  return zombie->second;
}

void Zombie::Spawn(float startX, float startY, Type &type, int seed) noexcept {
  // 1) Pick the right file path
  const char *path =
      (type == Type::Bloater  ? "../assets/graphics/bloater.png"
       : type == Type::Chaser ? "../assets/graphics/chaser.png"
                              : "../assets/graphics/crawler.png");

  // 2) Grab & store the texture pointer
  sf::Texture &tex = TextureHolder::GetTexture(path);
  mTexture = tex;

  // 3) Give it to the sprite (reset its rect so size matches texture)
  mSprite.setTexture(mTexture, /* resetRect = */ true);

  // 4) Position it
  mPosition = {startX, startY};
  mSprite.setPosition(mPosition);

  // 5) Stats
  auto stats = GetZombieStat(type);
  mSpeed = stats.speed;
  mHealth = stats.health;
  mAlive = true;

  // Create unique speed for each zombie type
  srand((int)time(0) * seed);
  float modifier = (rand() % MAX_VARIANCE) + OFFSET;
  modifier /= 100;
  mSpeed *= modifier;

  mPosition.x = startX;
  mPosition.y = startY;
  mSprite.setOrigin({25, 25});
  mSprite.setPosition(mPosition);
}

void Zombie::Update(float elapsedTime, sf::Vector2f &playerLocation) noexcept {
  float playerX = playerLocation.x;
  float playerY = playerLocation.y;

  // Update the zombie position variables
  // The zombie “walks” straight toward the player at a constant speed (though
  // note you’ll move diagonally faster because you apply both X and Y moves
  // independently).
  if (playerX > mPosition.x) {
    mPosition.x = mPosition.x + mSpeed * elapsedTime;
  }
  if (playerY > mPosition.y) {
    mPosition.y = mPosition.y + mSpeed * elapsedTime;
  }

  if (playerX < mPosition.x) {
    mPosition.x = mPosition.x - mSpeed * elapsedTime;
  }
  if (playerY < mPosition.y) {
    mPosition.y = mPosition.y - mSpeed * elapsedTime;
  }

  mSprite.setPosition(mPosition);
  float angleRad = std::atan2(playerY - mPosition.y, playerX - mPosition.x);

  mSprite.setRotation(sf::radians(angleRad));
}
