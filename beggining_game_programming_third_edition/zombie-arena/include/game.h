#pragma once

#include "SFML/Graphics.hpp"
#include "arena.h"
#include "bullet.h"
#include "player.h"
#include <tuple>

using CloseHandler = std::function<void(const sf::Event::Closed &)>;
using KeyPressHandler = std::function<void(const sf::Event::KeyPressed &)>;
using Events = std::tuple<CloseHandler, KeyPressHandler>;

struct Game {
  enum class State { Paused, LevelingUp, GameOver, Playing };
  Game();
  void Run();
  void Update();
  void Draw();
  // using auto also works here instead of Events.
  Events Events();
  void UpdateHUD();

  sf::RenderWindow mWindow;
  State mState;
  sf::Vector2f mResolution;
  sf::View mMainView;
  sf::Clock mClock;
  // How long has the PLAYING state been active
  sf::Time mGameTimeTotal;
  // Where is the mouse in
  // relation to world coordinates
  sf::Vector2f mMouseWorldPosition;
  // Where is the mouse in
  // relation to screen coordinates
  sf::Vector2f mMouseScreenPosition;
  // Create an instance of the Player class
  Player mPlayer;
  // The boundaries of the arena
  sf::IntRect mArena;

  sf::VertexArray mBackground;
  sf::Texture mTextureBackground;

  int mNumZombies;
  int mNumZombiesAlive;
  Horde mHorde;

  Bullet mBullets[100];
  int mCurrentBullet;
  int mBulletsSpare;
  int mBulletsInClip;
  int mClipSize;
  float mFireRate;
  sf::Time mTriggerLastPressed;

  sf::Texture mCrosshairTexture;
  sf::Sprite mCrosshairSprite;

  int mScore;
  int mHighScore;

  sf::Texture mGameOverTexture;
  sf::Sprite mGameOverSprite;
  sf::View mHudView;
  sf::Sprite mSpriteAmmoIcon;
  sf::Texture mTextureAmmoIcon;
  sf::Font mFont;
  sf::Text mPausedText;
  sf::Text mGameOverText;
  sf::Text mLevelUpText;
  sf::Text mAmmoText;
  sf::Text mScoreText;
  sf::Text mHiScoreText;
  sf::Text mZombiesRemainingText;
  sf::Text mWaveNumberText;
  sf::RectangleShape mHealthBar;
  int mWave;
  int mFramesSinceLastHUDUpdate;
  int mFpsMeasurementFrameInterval;
};
