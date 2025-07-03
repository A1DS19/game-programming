#include "actor.hpp"
#include "events.hpp"
#include <cstdlib>
#include <ctime>

int main() {
  constexpr auto screenDims = sf::Vector2u{1920, 1080};
  sf::RenderWindow window(sf::VideoMode(screenDims), "Timber");

  Actor background("../assets/graphics/background.png", {0, 0});
  Actor tree("../assets/graphics/tree2.png", {810, 0});
  Actor bee("../assets/graphics/bee.png", {0, 600});
  Actor cloud1("../assets/graphics/cloud.png", {0, 0});
  Actor cloud2("../assets/graphics/cloud.png", {0, 180});
  Actor cloud3("../assets/graphics/cloud.png", {0, 370});

  sf::Clock clock;
  while (window.isOpen()) {
    while (const std::optional maybeEvent = window.pollEvent()) {
      const sf::Event &event = *maybeEvent;

      eventHandler(event, window);
      keyPressedHandler(event, window);
    }

    sf::Time dt = clock.restart();

    if (!bee.active) {
      bee.Initialize(1000, 500, 500, 500, 1940);
    } else {
      bee.Move(dt, -100, Actor::Direction::ELeft);
    }

    if (!cloud1.active) {
      cloud1.Initialize(250, 300, 150, -200);
    } else {
      cloud1.Move(dt, 1920, Actor::Direction::ERigth);
    }

    if (!cloud2.active) {
      cloud2.Initialize(280, 450, 150, -200);
    } else {
      cloud2.Move(dt, 1920, Actor::Direction::ERigth);
    }

    if (!cloud3.active) {
      cloud3.Initialize(200, 550, 10, -200);
    } else {
      cloud3.Move(dt, 1920, Actor::Direction::ERigth);
    }

    window.clear(sf::Color::Black);

    // draw sprites here
    window.draw(*background.sprite);
    window.draw(*tree.sprite);
    window.draw(*bee.sprite);
    window.draw(*cloud1.sprite);
    window.draw(*cloud2.sprite);
    window.draw(*cloud3.sprite);

    window.display();
  }
  return 0;
}
