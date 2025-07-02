#include "events.hpp"

void eventHandler(const sf::Event &event, sf::RenderWindow &window) {
  if (event.is<sf::Event::Closed>()) {
    window.close();
  }
}

void keyPressedHandler(const sf::Event &event, sf::RenderWindow &window) {
  if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
      window.close();
    }
  }
}
