#pragma once

#include <SFML/Graphics.hpp>
#include <ostream>

class Player {
public:
  enum class Movement { Up, Down, Left, Right };

  Player();

  void Spawn(const sf::IntRect &arena, const sf::Vector2f &resolution,
             int tileSize) noexcept;
  void ResetPlayerStats() noexcept;
  bool Hit(const sf::Time &timeHit) noexcept;

  const sf::Time &GetLastHitTime() const noexcept { return mLastHit; }
  sf::FloatRect GetPosition() const noexcept {
    return mSprite.getGlobalBounds();
  }
  const sf::Vector2f &GetCenter() const noexcept { return mPosition; }
  sf::Angle GetRotation() const noexcept { return mSprite.getRotation(); }

  const sf::Sprite &GetSprite() const noexcept { return mSprite; }

  void Move(Movement m) noexcept;
  void Stop(Movement m) noexcept;
  void Update(float dt, const sf::Vector2i &mousePosition);
  void UpgradeSpeed() noexcept;
  void UpgradeMaxHealth() noexcept;
  void IncreaseHealthLevel(unsigned int amount) noexcept;
  int GetHealth() const noexcept { return mHealth; }

  friend std::ostream &operator<<(std::ostream &os, const Player &p);

private:
  inline static constexpr float START_SPEED = 200.f;
  inline static constexpr float START_HEALTH = 100.f;

  sf::Texture mTexture;
  sf::Sprite mSprite;

  sf::Vector2f mPosition{0.f, 0.f};
  sf::Vector2f mResolution{0.f, 0.f};
  sf::IntRect mArena{};
  int mTileSize{0};

  bool mUpPressed{false}, mDownPressed{false}, mLeftPressed{false},
      mRightPressed{false};

  int mHealth{static_cast<int>(START_HEALTH)};
  int mMaxHealth{static_cast<int>(START_HEALTH)};
  sf::Time mLastHit{};
  float mSpeed{START_SPEED};
};
