#pragma once

#include "SFML/Graphics.hpp"
#include <map>

class Zombie {
public:
  struct Stats {
    float speed;
    float health;
  };
  enum class Type { Bloater, Chaser, Crawler };
  Zombie() noexcept;
  // Handle when bullet hits zombie
  bool Hit() noexcept;
  // Is zombie alive?
  const bool IsAlive() const noexcept { return mAlive; }
  // Spawn a new zombie
  void Spawn(float startX, float startY, Type &type, int seed) noexcept;
  const sf::FloatRect GetPosition() const noexcept {
    return mSprite.getGlobalBounds();
  }
  sf::Sprite GetSprite() noexcept { return mSprite; }
  void Update(float elapsedTime, sf::Vector2f &playerLocation) noexcept;

private:
  const std::map<Type, Stats> zombieStats = {{Type::Bloater, {40.f, 5.f}},
                                             {Type::Chaser, {80.f, 1.f}},
                                             {Type::Crawler, {20.f, 3.f}}};
  const Stats GetZombieStat(Type type) noexcept;
  // Speed variance
  const int MAX_VARIANCE = 30.f;
  const int OFFSET = 101 - MAX_VARIANCE;

  sf::Vector2f mPosition;
  sf::Texture mTexture;
  sf::Sprite mSprite;
  float mSpeed;
  float mHealth;
  bool mAlive;
};
