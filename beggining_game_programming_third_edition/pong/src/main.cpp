#include "SFML/Graphics.hpp"
#include "bat.h"
#include "game.h"
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode(Game::screenDims), "Pong");
  Bat bat(Game::screenDims.x / 2.0f, Game::screenDims.y / 2.0f + 150.0f);

  unsigned int score = 0;
  unsigned int lives = 3;

  sf::Font font;
  if (!font.openFromFile("../assets/fonts/DS-DIGIT.TTF")) {
    std::cerr << "could not load font" << std::endl;
  }
  sf::Text hud(font);
  hud.setCharacterSize(75);
  hud.setFillColor(sf::Color::White);
  hud.setPosition({20, 20});

  const auto onClose = [&window](const sf::Event::Closed &) { window.close(); };

  const auto onKeyPress = [&](const sf::Event::KeyPressed &keyPress) {
    if (keyPress.scancode == sf::Keyboard::Scancode::Escape) {
      window.close();
    }

    if (keyPress.scancode == sf::Keyboard::Scancode::Left) {
      bat.Move(Bat::Direction::Left);
    }
    if (keyPress.scancode == sf::Keyboard::Scancode::Right) {
      bat.Move(Bat::Direction::Right);
    }
  };

  const auto onKeyRelease = [&](const sf::Event::KeyReleased &keyReleased) {
    if (keyReleased.scancode == sf::Keyboard::Scancode::Left) {
      bat.Stop();
    }
    if (keyReleased.scancode == sf::Keyboard::Scancode::Right) {
      bat.Stop();
    }
  };

  sf::Clock clock;
  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPress, onKeyRelease);

    std::stringstream ss;
    ss << "Score:" << score << "  "
       << "Lives :" << lives;
    hud.setString(ss.str());

    sf::Time dt = clock.restart();
    bat.Update(dt);

    window.clear(sf::Color::Black);
    window.draw(hud);
    window.draw(bat.GetShape());
    window.display();
  }

  return 0;
}
