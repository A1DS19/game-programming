#include "SFML/Graphics.hpp"
#include "ball.h"
#include "bat.h"
#include "game.h"
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode(Game::screenDims), "Pong");
  Bat bat(Game::screenDims.x / 2.0f, Game::screenDims.y / 2.0f + 150.0f);
  Ball ball(Game::screenDims.x / 2.0f, 0);

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
      bat.Move(Game::Direction::Left);
    }
    if (keyPress.scancode == sf::Keyboard::Scancode::Right) {
      bat.Move(Game::Direction::Right);
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

    auto bounds = ball.GetShape().getGlobalBounds();
    float left = bounds.position.x;
    float top = bounds.position.y;
    float right = left + bounds.size.x;
    float bottom = top + bounds.size.y;

    // Fell off bottom?
    if (bottom > Game::screenDims.y) {
      ball.Rebound(Ball::ReboundPosition::Bottom);
      lives--;
      if (lives < 1) {
        score = 0;
        lives = 3;
      }
    }

    // Hit top wall?
    if (top < 0) {
      ball.Rebound(Ball::ReboundPosition::Top);
      score++;
    }

    // Hit left or right walls?
    if (left < 0 || right > window.getSize().x) {
      ball.Rebound(Ball::ReboundPosition::Sides);
    }

    // Hit the bat?
    if (auto inter =
            bounds.findIntersection(bat.GetShape().getGlobalBounds())) {
      ball.Rebound(Ball::ReboundPosition::Bat);
      // nudge the ball out so it doesn’t stick into the paddle:
      ball.GetShape().move({0, -inter->size.y});
    }

    sf::Time dt = clock.restart();
    bat.Update(dt);
    ball.Update(dt);

    window.clear(sf::Color::Black);
    window.draw(hud);
    window.draw(bat.GetShape());
    window.draw(ball.GetShape());
    window.display();
  }

  return 0;
}
