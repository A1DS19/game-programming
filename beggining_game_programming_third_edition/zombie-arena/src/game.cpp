#include "game.h"
#include <tuple>

Game::Game() {
  mWindow = sf::RenderWindow(sf::VideoMode(Game::ScreenDims), "Zombie Arena");
}

void Game::Update() {}

void Game::Draw() {}

Events Game::Events() {
  const auto onClose = [&](const sf::Event::Closed &) { mWindow.close(); };

  const auto onKeyPress = [&](const sf::Event::KeyPressed &keyPress) {
    if (keyPress.scancode == sf::Keyboard::Scancode::Escape) {
      mWindow.close();
    }
  };

  return std::make_tuple(onClose, onKeyPress);
}

void Game::Run() {
  auto [onClose, onKeyPress] = Game::Events();

  while (mWindow.isOpen()) {
    mWindow.handleEvents(onClose, onKeyPress);

    Game::Update();

    mWindow.clear(sf::Color::Black);
    Game::Draw();
    mWindow.display();
  }
}
