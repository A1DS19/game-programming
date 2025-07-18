#include "game.h"
#include "arena.h"
#include "player.h"
#include <iostream>

Game::Game()
    : mState(Game::State::GameOver), mClock(sf::Clock()),
      mGameTimeTotal(sf::Time()), mMouseWorldPosition({0, 0}),
      mMouseScreenPosition({0, 0}), mPlayer(Player()), mArena(), mBackground(),
      mTextureBackground() {
  auto x = sf::VideoMode::getDesktopMode().size.x;
  auto y = sf::VideoMode::getDesktopMode().size.y;

  mResolution = {float(x), float(y)};
  mWindow = sf::RenderWindow(sf::VideoMode({x, y}), "Zombie Arena");
  mMainView = sf::View(sf::FloatRect({0.f, 0.f}, {float(x), float(y)}));

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

    mPlayer.Update(dtAsSeconds, pixelPos);

    sf::Vector2f playerPosition(mPlayer.GetCenter());

    // recenter camera on view when moving
    mMainView.setCenter(mPlayer.GetCenter());
  }
}

void Game::Draw() {
  mWindow.clear();

  mWindow.setView(mMainView);
  mWindow.draw(mBackground, &mTextureBackground);
  mWindow.draw(mPlayer.GetSprite());

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
