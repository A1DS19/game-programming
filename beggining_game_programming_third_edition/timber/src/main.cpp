#include "SFML/Graphics.hpp"
#include <iostream>

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

int main() {
  constexpr auto screenDims = sf::Vector2u{1920, 1080};
  sf::RenderWindow window(sf::VideoMode(screenDims), "Timber");

  window.setVerticalSyncEnabled(true);

  sf::Texture textureBackground;
  if (!textureBackground.loadFromFile(
          "../assets/graphics/background.png", false,
          sf::IntRect({0, 0},                                   // position
                      static_cast<sf::Vector2<int>>(screenDims) // size
                      ))) {
    std::cerr << "could not load texture" << std::endl;
    return 1;
  }

  sf::Sprite sprite(textureBackground);

  while (window.isOpen()) {
    while (const std::optional maybeEvent = window.pollEvent()) {
      const sf::Event &event = *maybeEvent;

      eventHandler(event, window);
      keyPressedHandler(event, window);
    }

    window.clear(sf::Color::Black);

    // draw sprites here
    window.draw(sprite);

    window.display();
  }
  return 0;
}
