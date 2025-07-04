#include "actor.hpp"
#include "events.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  constexpr auto screenDims = sf::Vector2u{1920, 1080};
  sf::RenderWindow window(sf::VideoMode(screenDims), "Timber");

  sf::Clock clock;
  bool paused = true;
  int score = 0;
  sf::Font font;

  std::string fontFilePath = "../assets/fonts/KOMIKAP_.ttf";
  if (!font.openFromFile(fontFilePath)) {
    std::cerr << "font: " << fontFilePath << "not found" << std::endl;
  }

  sf::Text startGameText(font);
  startGameText.setCharacterSize(75);
  startGameText.setFillColor(sf::Color::White);
  startGameText.setString("Press Enter to start");

  // 1) Get the text’s local bounds (position & size):
  auto bounds = startGameText.getLocalBounds();
  //    bounds.position == top-left within the glyphs
  //    bounds.size     == width/height of the glyphs

  // 2) Compute and set the origin to the very center of those bounds:
  sf::Vector2f localCenter{bounds.position.x + bounds.size.x * 0.5f,
                           bounds.position.y + bounds.size.y * 0.5f};
  startGameText.setOrigin(localCenter);

  // 3) Get the window size and position the text at its center:
  sf::Vector2u winSize = window.getSize();
  sf::Vector2f windowCenter{winSize.x / 2.0f, winSize.y / 2.0f};
  startGameText.setPosition(windowCenter);

  sf::Text scoreText(font);
  scoreText.setCharacterSize(100);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setString("Score = 0");
  scoreText.setPosition({20, 20});

  Actor background("../assets/graphics/background.png", {0, 0});
  Actor tree("../assets/graphics/tree2.png", {810, 0});
  Actor bee("../assets/graphics/bee.png", {0, 600});
  Actor cloud1("../assets/graphics/cloud.png", {0, 0});
  Actor cloud2("../assets/graphics/cloud.png", {0, 180});
  Actor cloud3("../assets/graphics/cloud.png", {0, 370});

  while (window.isOpen()) {
    while (const std::optional maybeEvent = window.pollEvent()) {
      const sf::Event &event = *maybeEvent;

      eventHandler(event, window);
      keyPressedHandler(event, window);
    }

    if (!paused) {
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
    }

    window.clear(sf::Color::Black);

    // draw sprites here
    window.draw(*background.sprite);
    window.draw(*tree.sprite);
    window.draw(*bee.sprite);
    window.draw(*cloud1.sprite);
    window.draw(*cloud2.sprite);
    window.draw(*cloud3.sprite);
    window.draw(scoreText);

    if (paused) {
      window.draw(startGameText);
    }

    window.display();
  }
  return 0;
}
