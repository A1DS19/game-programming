#pragma once

#include "InputReciever.h"
#include "SFML/Graphics.hpp"
#include "Update.h"
#include "memory"

class PlayerUpdate : public Update {
private:
  const float PLAYER_WIDTH = 20.f;
  const float PLAYER_HEIGHT = 16.f;
  sf::FloatRect mPosition;

  bool *mIsPaused = nullptr;
  float mGravity = 165;
  float mRunSpeed = 150;
  float mBoostSpeed = 250;
  InputReciever mInputReciever;

  sf::Clock mJumpClock;
  bool mSpaceHeldDown = false;
  float mJumpDuration = .50;
  float mJumpSpeed = 400;

public:
  bool mRightIsHeldDown = false;
  bool mLeftIsHeldDown = false;
  bool mBoostIsHeldDown = false;

  bool mIsGrounded;
  bool mInJump = false;

  sf::FloatRect *GetPositionPointer() noexcept { return &mPosition; }
  bool *GetGroundedPointer() noexcept { return &mIsGrounded; };
  void HandleInput() noexcept;
  InputReciever *GetInputReceiver() noexcept { return &mInputReciever; };

  // From Update : Component
  void Assemble(std::shared_ptr<LevelUpdate> levelUpdate,
                std::shared_ptr<PlayerUpdate> playerUpdate) override;

  void Updatefn(float fps) override;
};
