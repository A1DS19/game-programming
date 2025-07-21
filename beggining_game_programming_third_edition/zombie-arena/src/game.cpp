#include "game.h"
#include "arena.h"
#include "player.h"
#include <iostream>

Game::Game()
    : mState(Game::State::GameOver), mClock(sf::Clock()),
      mGameTimeTotal(sf::Time()), mMouseWorldPosition({0, 0}),
      mMouseScreenPosition({0, 0}), mPlayer(Player()), mArena(), mBackground(),
      mTextureBackground(), mNumZombies(0), mNumZombiesAlive(0),
      mCurrentBullet(0), mBulletsSpare(24), mBulletsInClip(6), mClipSize(6),
      mFireRate(1), mCrosshairTexture(), mCrosshairSprite(mCrosshairTexture) {
  auto x = sf::VideoMode::getDesktopMode().size.x;
  auto y = sf::VideoMode::getDesktopMode().size.y;

  mResolution = {float(x), float(y)};
  mWindow = sf::RenderWindow(sf::VideoMode({x, y}), "Zombie Arena");
  mMainView = sf::View(sf::FloatRect({0.f, 0.f}, {float(x), float(y)}));
  mWindow.setMouseCursorVisible(true);

  if (!mCrosshairTexture.loadFromFile("../assets/graphics/crosshair.png")) {
    std::cerr << "could not load crosshair" << std::endl;
  }
  mCrosshairSprite.setTexture(mCrosshairTexture, true);
  mCrosshairSprite.setOrigin({25, 25});

  if (!mTextureBackground.loadFromFile(
          "../assets/graphics/background_sheet.png")) {
    std::cerr << "Background sheet loading error" << std::endl;
  }
}

void Game::Update() {
  if (mState == Game::State::Playing) {

    // Update delta time
    sf::Time dt = mClock.restart();

    // Update game total time
    mGameTimeTotal += dt;

    float dtAsSeconds = dt.asSeconds();

    sf::Vector2i pixelPos = sf::Mouse::getPosition(mWindow);

    mMouseScreenPosition = sf::Vector2f(pixelPos);

    mMouseWorldPosition = mWindow.mapPixelToCoords(pixelPos, mMainView);

    mCrosshairSprite.setPosition(mMouseWorldPosition);

    mPlayer.Update(dtAsSeconds, pixelPos);

    sf::Vector2f playerPosition(mPlayer.GetCenter());

    // recenter camera on view when moving
    mMainView.setCenter(mPlayer.GetCenter());

    for (size_t i = 0; i < mNumZombies; i++) {
      if (mHorde[i]->IsAlive()) {
        mHorde[i]->Update(dtAsSeconds, playerPosition);
      }
    }

    for (size_t i = 0; i < 100; i++) {
      if (mBullets[i].IsInFlight()) {
        mBullets[i].Update(dtAsSeconds);
      }
    }
  }
}

void Game::Draw() {
  mWindow.clear();

  mWindow.setView(mMainView);
  mWindow.draw(mBackground, &mTextureBackground);
  mWindow.draw(mPlayer.GetSprite());
  mWindow.draw(mCrosshairSprite);

  for (size_t i = 0; i < mNumZombies; i++) {
    mWindow.draw(mHorde[i]->GetSprite());
  }
  for (size_t i = 0; i < 100; i++) {
    if (mBullets[i].IsInFlight()) {
      mWindow.draw(mBullets[i].GetShape());
    }
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
