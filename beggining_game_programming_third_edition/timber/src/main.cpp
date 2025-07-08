#include "actor.hpp"
#include "axe.hpp"
#include "branch.hpp"
#include "log.hpp"
#include "player.hpp"
#include "rip.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
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

  sf::Text centerMessage(font);
  centerMessage.setCharacterSize(75);
  centerMessage.setFillColor(sf::Color::White);
  centerMessage.setString("Press Enter to start");

  const auto centerGameMessage = [&centerMessage, &window]() {
    // 1) Get the text’s local bounds (position & size):
    auto bounds = centerMessage.getLocalBounds();
    //    bounds.position == top-left within the glyphs
    //    bounds.size     == width/height of the glyphs

    // 2) Compute and set the origin to the very center of those bounds:
    sf::Vector2f localCenter{bounds.position.x + bounds.size.x * 0.5f,
                             bounds.position.y + bounds.size.y * 0.5f};
    centerMessage.setOrigin(localCenter);

    // 3) Get the window size and position the text at its center:
    sf::Vector2u winSize = window.getSize();
    sf::Vector2f windowCenter{winSize.x / 2.0f, winSize.y / 2.0f};
    centerMessage.setPosition(windowCenter);
  };
  centerGameMessage();

  sf::Text scoreText(font);
  scoreText.setCharacterSize(100);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setString("Score = 0");
  scoreText.setPosition({20, 20});

  sf::RectangleShape timeBar;
  float timeBarStartW = 400.0f;
  float timeBarH = 50.0f;
  timeBar.setSize({timeBarStartW, timeBarH});
  timeBar.setFillColor(sf::Color::Red);
  auto size = window.getSize();                      // e.g. {1920, 1080}
  float x = ((size.x - timeBarStartW) + 480) * 0.5f; // center horizontally
  float y = (size.y - 10.0f) - timeBarH; // align top of bar at bottom of window
  timeBar.setPosition({x, y});

  sf::Time gameTimeTotal;
  float timeRemaining = 6.0f;
  float timeBarWPerSecond = timeBarStartW / timeRemaining;

  Branches::CreateBranches();

  Actor background("../assets/graphics/background.png", {0, 0});
  Actor tree("../assets/graphics/tree2.png", {810, -100});
  Actor bee("../assets/graphics/bee.png", {0, 500});
  Actor cloud1("../assets/graphics/cloud.png", {0, 0});
  Actor cloud2("../assets/graphics/cloud.png", {0, 160});
  Actor cloud3("../assets/graphics/cloud.png", {0, 350});
  Axe axe("../assets/graphics/axe.png", {700, 720});
  Player player("../assets/graphics/player.png", {580, 600});
  Log log("../assets/graphics/log.png", {810, 620});
  Rip rip("../assets/graphics/rip.png", {600, 680});

  const auto onClose = [&window](const sf::Event::Closed &) { window.close(); };

  bool acceptInput = false;
  const auto onKeyPressed = [&](const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scancode::Escape) {
      window.close();
    }

    if (keyPressed.scancode == sf::Keyboard::Scancode::Enter &&
        paused == true) {
      paused = false;
      score = 0;
      timeRemaining = 6.0f;

      // Make all branches disappear
      for (unsigned int i = 0; i < Branches::Constants::MAX; i++) {
        Branches::branchPositions[i] = Game::Side::NONE;
      }

      // hide gravestone
      rip.sprite->setPosition({675, 2000});

      // move player to position
      player.sprite->setPosition({580, 600});
      axe.sprite->setPosition({700, 720});

      acceptInput = true;
    }

    if (acceptInput) {
      if (keyPressed.scancode == sf::Keyboard::Scancode::Right && !paused) {
        player.side = Game::Side::RIGHT;
        score++;
        timeRemaining += (2.0f / score) + 0.15f;
        axe.sprite->setPosition(
            {Axes::AXE_POSITION_RIGHT, axe.sprite->getPosition().y});
        player.sprite->setPosition({1200, 600});
        Branches::UpdateBranches(score);
        log.sprite->setPosition({810, 720});
        log.speedX = -5000;
        log.active = true;
        acceptInput = false;
      }

      if (keyPressed.scancode == sf::Keyboard::Scancode::Left && !paused) {
        player.side = Game::Side::LEFT;
        score++;
        timeRemaining += (2.0f / score) + 0.15f;
        axe.sprite->setPosition(
            {Axes::AXE_POSITION_LEFT, axe.sprite->getPosition().y});
        player.sprite->setPosition({580, 600});
        Branches::UpdateBranches(score);
        log.sprite->setPosition({810, 720});
        log.speedX = 5000;
        log.active = true;
        acceptInput = false;
      }
    }
  };

  const auto onKeyReleased = [&](const sf::Event::KeyReleased &keyReleased) {
    if (!paused) {
      acceptInput = true;
      axe.sprite->setPosition({2000, axe.sprite->getPosition().y});
    }
  };

  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, onKeyReleased);

    std::stringstream ss;
    ss << "Score = " << score;
    scoreText.setString(ss.str());

    for (unsigned int i = 0; i < Branches::Constants::MAX; i++) {
      float height = i * 150;

      if (Branches::branchPositions[i] == Game::Side::LEFT) {
        Branches::branches[i].sprite->setPosition({810, height});
        Branches::branches[i].sprite->setRotation(sf::degrees(180.0f));
      } else if (Branches::branchPositions[i] == Game::Side::RIGHT) {
        Branches::branches[i].sprite->setPosition({1100, height});
        Branches::branches[i].sprite->setRotation(sf::degrees(0.0f));
      } else {
        Branches::branches[i].sprite->setPosition({3000, height});
      }
    }

    if (!paused) {
      sf::Time dt = clock.restart();

      timeRemaining -= dt.asSeconds();
      timeBar.setSize({timeBarWPerSecond * timeRemaining, timeBarH});

      if (timeRemaining <= 0.0f) {
        paused = true;
        centerMessage.setString("Out of time! Press Enter to restart");
        centerGameMessage();
      }

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

      if (log.active) {
        log.sprite->setPosition(
            {log.sprite->getPosition().x + (log.speedX * dt.asSeconds()),
             log.sprite->getPosition().y + (log.speedY * dt.asSeconds())});

        if (log.sprite->getPosition().x < -100 ||
            log.sprite->getPosition().x > 2000) {
          log.active = false;
          log.sprite->setPosition({810, 620});
        }
      }

      if (Branches::branchPositions[5] == player.side) {
        // death
        paused = true;
        acceptInput = false;

        rip.sprite->setPosition({580, 600});
        player.sprite->setPosition({2000, 600});

        centerMessage.setString("SQUISHED!");
        centerGameMessage();
      }
    }

    window.clear(sf::Color::Black);

    // draw sprites here
    window.draw(*background.sprite);

    if (!paused) {
      for (unsigned int i = 0; i < Branches::Constants::MAX; i++) {
        window.draw(*Branches::branches[i].sprite);
      }
      window.draw(*tree.sprite);
      window.draw(*bee.sprite);
      window.draw(*cloud1.sprite);
      window.draw(*cloud2.sprite);
      window.draw(*cloud3.sprite);
      window.draw(*player.sprite);
      window.draw(*axe.sprite);
      window.draw(*log.sprite);
      window.draw(*rip.sprite);
      window.draw(timeBar);
    }

    if (paused) {
      window.draw(centerMessage);
    }

    window.draw(scoreText);
    window.display();
  }
  return 0;
}
