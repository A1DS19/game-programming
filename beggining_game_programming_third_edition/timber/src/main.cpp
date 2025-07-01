#include "SFML/Graphics.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Timber");

  window.setVerticalSyncEnabled(true);

  sf::Texture texture("../assets/graphics/background.png", false,
                      sf::IntRect({0, 0},      // position
                                  {1920, 1080} // size
                                  ));
  sf::Sprite sprite(texture);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *keyPressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        }
      }
    }

    window.clear(sf::Color::Black);

    // draw sprites here
    window.draw(sprite);

    window.display();
  }
  return 0;
}
