#include "game.h"
#include "arena.h"
#include "player.h"
#include "sstream"
#include <iostream>

Game::Game()
    : mState(Game::State::GameOver), mClock(), mGameTimeTotal(sf::Time()),
      mMouseWorldPosition({0, 0}), mMouseScreenPosition({0, 0}), mPlayer(),
      mArena(), mBackground(), mTextureBackground(), mNumZombies(0),
      mNumZombiesAlive(0), mCurrentBullet(0), mBulletsSpare(24),
      mBulletsInClip(6), mClipSize(6), mFireRate(1.f),
      mTriggerLastPressed(sf::Time()), mCrosshairTexture(),
      mCrosshairSprite(mCrosshairTexture), mScore(0), mHighScore(0),
      mGameOverTexture(), mGameOverSprite(mGameOverTexture),
      mHudView(sf::FloatRect({0, 0}, {1920, 1080})), mTextureAmmoIcon(),
      mSpriteAmmoIcon(mTextureAmmoIcon), mFont(), mPausedText(mFont),
      mGameOverText(mFont), mLevelUpText(mFont), mAmmoText(mFont),
      mScoreText(mFont), mHiScoreText(mFont), mZombiesRemainingText(mFont),
      mWaveNumberText(mFont), mHealthBar(), mWave(0),
      mFramesSinceLastHUDUpdate(0), mFpsMeasurementFrameInterval(1000) {

  auto x = sf::VideoMode::getDesktopMode().size.x;
  auto y = sf::VideoMode::getDesktopMode().size.y;

  mResolution = {float(x), float(y)};
  mWindow.create(sf::VideoMode({x, y}), "Zombie Arena");
  mMainView = sf::View(sf::FloatRect({0.f, 0.f}, {float(x), float(y)}));
  mWindow.setMouseCursorVisible(true);

  if (!mFont.openFromFile("../assets/fonts/zombiecontrol.ttf")) {
    std::cerr << "Could not load font" << std::endl;
  }

  if (!mCrosshairTexture.loadFromFile("../assets/graphics/crosshair.png")) {
    std::cerr << "Could not load crosshair" << std::endl;
  }
  mCrosshairSprite.setTexture(mCrosshairTexture, true);
  mCrosshairSprite.setOrigin({25, 25});

  if (!mTextureBackground.loadFromFile(
          "../assets/graphics/background_sheet.png")) {
    std::cerr << "Background sheet loading error" << std::endl;
  }

  if (!mTextureAmmoIcon.loadFromFile("../assets/graphics/ammo_icon.png")) {
    std::cerr << "Could not load ammo icon texture" << std::endl;
  }
  mSpriteAmmoIcon.setTexture(mTextureAmmoIcon);
  mSpriteAmmoIcon.setPosition({20, 980});

  mPausedText.setCharacterSize(155);
  mPausedText.setFillColor(sf::Color::White);
  mPausedText.setPosition({400, 400});
  mPausedText.setString("Press Enter \nto continue");

  mGameOverText.setCharacterSize(125);
  mGameOverText.setFillColor(sf::Color::White);
  mGameOverText.setPosition({250, 850});
  mGameOverText.setString("Press Enter to play");

  std::stringstream levelUpStream;
  levelUpStream << "1- Increased rate of fire"
                   "\n2- Increased clip size(next reload)"
                   "\n3- Increased max health"
                   "\n4- Increased run speed"
                   "\n5- More and better health pickups"
                   "\n6- More and better ammo pickups";
  mLevelUpText.setCharacterSize(80);
  mLevelUpText.setFillColor(sf::Color::White);
  mLevelUpText.setPosition({150, 250});
  mLevelUpText.setString(levelUpStream.str());

  mAmmoText.setCharacterSize(55);
  mAmmoText.setFillColor(sf::Color::White);
  mAmmoText.setPosition({200, 980});

  mScoreText.setCharacterSize(55);
  mScoreText.setFillColor(sf::Color::White);
  mScoreText.setPosition({20, 0});

  mHiScoreText.setCharacterSize(55);
  mHiScoreText.setFillColor(sf::Color::White);
  mHiScoreText.setPosition({1400, 0});

  mZombiesRemainingText.setCharacterSize(55);
  mZombiesRemainingText.setFillColor(sf::Color::White);
  mZombiesRemainingText.setPosition({1500, 980});
  mZombiesRemainingText.setString("Zombies: 100");

  mWaveNumberText.setCharacterSize(55);
  mWaveNumberText.setFillColor(sf::Color::White);
  mWaveNumberText.setPosition({1250, 980});
  mWaveNumberText.setString("Wave: 0");

  mHealthBar.setFillColor(sf::Color::Red);
  mHealthBar.setPosition({450, 980});
}

void Game::UpdateHUD() {
  mHealthBar.setSize(sf::Vector2f(mPlayer.GetHealth() * 3.f, 50.f));

  mFramesSinceLastHUDUpdate++;
  if (mFramesSinceLastHUDUpdate > mFpsMeasurementFrameInterval) {
    std::stringstream ssAmmo, ssScore, ssHiScore, ssWave, ssZombiesAlive;

    ssAmmo << mBulletsInClip << "/" << mBulletsSpare;
    mAmmoText.setString(ssAmmo.str());

    ssScore << "Score: " << mScore;
    mScoreText.setString(ssScore.str());

    ssHiScore << "Hi Score: " << mHighScore;
    mHiScoreText.setString(ssHiScore.str());

    ssWave << "Wave: " << mWave;
    mWaveNumberText.setString(ssWave.str());

    ssZombiesAlive << "Zombies: " << mNumZombiesAlive;
    mZombiesRemainingText.setString(ssZombiesAlive.str());

    mFramesSinceLastHUDUpdate = 0;
  }
}

void Game::Update() {
  if (mState == Game::State::Playing) {
    sf::Time dt = mClock.restart();
    mGameTimeTotal += dt;
    float dtAsSeconds = dt.asSeconds();

    sf::Vector2i pixelPos = sf::Mouse::getPosition(mWindow);
    mMouseScreenPosition = sf::Vector2f(pixelPos);
    mMouseWorldPosition = mWindow.mapPixelToCoords(pixelPos, mMainView);
    mCrosshairSprite.setPosition(mMouseWorldPosition);

    mPlayer.Update(dtAsSeconds, pixelPos);
    sf::Vector2f playerPosition(mPlayer.GetCenter());
    mMainView.setCenter(mPlayer.GetCenter());

    for (size_t i = 0; i < mNumZombies; i++) {
      if (mHorde[i] && mHorde[i]->IsAlive()) {
        mHorde[i]->Update(dtAsSeconds, playerPosition);
      }
    }

    for (size_t i = 0; i < 100; i++) {
      if (mBullets[i].IsInFlight()) {
        mBullets[i].Update(dtAsSeconds);
      }
    }

    for (int i = 0; i < 100; i++) {
      for (int j = 0; j < mNumZombies; j++) {
        if (mBullets[i].IsInFlight() && mHorde[j] && mHorde[j]->IsAlive()) {
          if (mBullets[i].GetPosition().findIntersection(
                  mHorde[j]->GetPosition())) {
            mBullets[i].Stop();
            if (mHorde[j]->Hit()) {
              mScore += 10;
              if (mScore >= mHighScore) {
                mHighScore = mScore;
              }
              --mNumZombiesAlive;
              if (mNumZombiesAlive == 0) {
                mState = Game::State::LevelingUp;
              }
            }
          }
        }
      }
    }

    UpdateHUD();
  }
}

void Game::Draw() {
  mWindow.clear();

  mWindow.setView(mMainView);
  mWindow.draw(mBackground, &mTextureBackground);
  mWindow.draw(mPlayer.GetSprite());
  mWindow.draw(mCrosshairSprite);

  for (size_t i = 0; i < mNumZombies; i++) {
    if (mHorde[i] && mHorde[i]->IsAlive()) {
      mWindow.draw(mHorde[i]->GetSprite());
    }
  }

  for (size_t i = 0; i < 100; i++) {
    if (mBullets[i].IsInFlight()) {
      mWindow.draw(mBullets[i].GetShape());
    }
  }

  mWindow.setView(mHudView);
  mWindow.draw(mSpriteAmmoIcon);
  mWindow.draw(mAmmoText);
  mWindow.draw(mScoreText);
  mWindow.draw(mHiScoreText);
  mWindow.draw(mHealthBar);
  mWindow.draw(mWaveNumberText);
  mWindow.draw(mZombiesRemainingText);

  if (mState == Game::State::Paused) {
    mWindow.draw(mPausedText);
  }

  if (mState == Game::State::GameOver) {
    mWindow.draw(mGameOverSprite);
    mWindow.draw(mGameOverText);
    mWindow.draw(mScoreText);
    mWindow.draw(mHiScoreText);
  }

  if (mState == Game::State::LevelingUp) {
    mWindow.draw(mGameOverSprite);
    mWindow.draw(mLevelUpText);
  }

  mWindow.display();
}

Events Game::Events() {
  const auto onClose = [&](const sf::Event::Closed &) { mWindow.close(); };

  const auto onKeyPress = [&](const sf::Event::KeyPressed &keyPress) {
    if (keyPress.scancode == sf::Keyboard::Scancode::Escape) {
      mWindow.close();
    }

    if (keyPress.scancode == sf::Keyboard::Scancode::Enter &&
        mState == Game::State::Playing) {
      mState = Game::State::Paused;
    }

    if (keyPress.scancode == sf::Keyboard::Scancode::Enter &&
        mState == Game::State::Paused) {
      mState = Game::State::Playing;
      mClock.restart();
    }

    if (keyPress.scancode == sf::Keyboard::Scancode::Enter &&
        mState == Game::State::GameOver) {
      mState = Game::State::LevelingUp;
    }

    if (mState == Game::State::Playing) {
      if (keyPress.scancode == sf::Keyboard::Scancode::W) {
        mPlayer.Move(Player::Movement::Up);
      } else {
        mPlayer.Stop(Player::Movement::Up);
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::S) {
        mPlayer.Move(Player::Movement::Down);
      } else {
        mPlayer.Stop(Player::Movement::Down);
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::A) {
        mPlayer.Move(Player::Movement::Left);
      } else {
        mPlayer.Stop(Player::Movement::Left);
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::D) {
        mPlayer.Move(Player::Movement::Right);
      } else {
        mPlayer.Stop(Player::Movement::Right);
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::R) {
        if (mBulletsSpare >= mClipSize) {
          mBulletsInClip = mClipSize;
          mBulletsSpare -= mClipSize;
        } else if (mBulletsSpare > 0) {
          mBulletsInClip = mBulletsSpare;
          mBulletsSpare = 0;
        } else {
          // More?
        }
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ||
          sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        if (mGameTimeTotal.asMilliseconds() -
                    mTriggerLastPressed.asMilliseconds() >
                1000.f / mFireRate &&
            mBulletsInClip > 0) {
          mBullets[mCurrentBullet].Shoot(
              mPlayer.GetCenter().x, mPlayer.GetCenter().y,
              mMouseWorldPosition.x, mMouseWorldPosition.y);
          mCurrentBullet++;
          if (mCurrentBullet > 99) {
            mCurrentBullet = 0;
          }
          mTriggerLastPressed = mGameTimeTotal;
          mBulletsInClip--;
        }
      }
    }

    if (mState == Game::State::LevelingUp) {
      if (keyPress.scancode == sf::Keyboard::Scancode::Num1) {
        mState = Game::State::Playing;
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::Num2) {
        mState = Game::State::Playing;
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::Num3) {
        mState = Game::State::Playing;
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::Num4) {
        mState = Game::State::Playing;
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::Num5) {
        mState = Game::State::Playing;
      }

      if (keyPress.scancode == sf::Keyboard::Scancode::Num6) {
        mState = Game::State::Playing;
      }

      if (mState == Game::State::Playing) {
        mArena.size.x = 500;
        mArena.size.y = 500;
        mArena.position.x = 0;
        mArena.position.y = 0;

        int tileSize = createBackground(mBackground, mArena);

        // Spawn  player in middle of the arena
        mPlayer.Spawn(mArena, mResolution, tileSize);

        mNumZombies = 10;
        mHorde = createHorde(mNumZombies, mArena);
        mNumZombiesAlive = mNumZombies;

        // Reset clock to prevent frame jump
        mClock.restart();
      }
    }
  };

  return std::make_tuple(onClose, onKeyPress);
}

void Game::Run() {
  auto [onClose, onKeyPress] = Game::Events();

  while (mWindow.isOpen()) {
    mWindow.handleEvents(onClose, onKeyPress);

    Game::Update();

    if (mState == Game::State::Playing) {
      Game::Draw();
    }
  }
}
