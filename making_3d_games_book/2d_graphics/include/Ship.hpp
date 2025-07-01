#pragma once

#include <cstdint>

#include "Actor.hpp"

class Game;
class Ship : public Actor {
public:
  Ship(Game *game);
  void UpdateActor(float deltaTime) override;
  void ProcessKeyboard(const uint8_t *state);
  float GetRightSpeed() const { return mRightSpeed; }
  float GetDownSpeed() const { return mDownSpeed; }

private:
  float mRightSpeed;
  float mDownSpeed;
};
