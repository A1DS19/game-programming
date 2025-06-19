#pragma once

#include <cstdint>

#include "Actor.hpp"

class Game;
class Ship : public Actor {
public:
  Ship(Game *game);

  void UpdateActor(float deltaTime) override;
  void ActorInput(const uint8_t *keyState) override;

private:
  float mLaserCooldown;
};
