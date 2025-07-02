#include "events.hpp"
#include "sprites.hpp"
#include <cstdlib>
#include <ctime>

int main() {
  constexpr auto screenDims = sf::Vector2u{1920, 1080};
  sf::RenderWindow window(sf::VideoMode(screenDims), "Timber");

  sf::Texture textureBackground;
  if (!loadTexture(textureBackground, "../assets/graphics/background.png")) {
    return 1;
  }

  sf::Texture textureTree;
  if (!loadTexture(textureTree, "../assets/graphics/tree2.png")) {
    return 1;
  }

  sf::Texture textureBee;
  if (!loadTexture(textureBee, "../assets/graphics/bee.png")) {
    return 1;
  }
  bool beeActive = false;
  float beeSpeed = 0.0f;

  sf::Texture textureCloud1;
  if (!loadTexture(textureCloud1, "../assets/graphics/cloud.png")) {
    return 1;
  }
  sf::Texture textureCloud2;
  if (!loadTexture(textureCloud2, "../assets/graphics/cloud.png")) {
    return 1;
  }
  sf::Texture textureCloud3;
  if (!loadTexture(textureCloud3, "../assets/graphics/cloud.png")) {
    return 1;
  }

  bool cloudActive1 = false;
  bool cloudActive2 = false;
  bool cloudActive3 = false;

  float cloudSpeed1 = 0.0f;
  float cloudSpeed2 = 0.0f;
  float cloudSpeed3 = 0.0f;

  sf::Sprite spriteBackground(textureBackground);
  spriteBackground.setPosition({0, 0});

  sf::Sprite spriteTree(textureTree);
  spriteTree.setPosition({810, 0});

  sf::Sprite spriteBee(textureBee);
  spriteBee.setPosition({0, 600});

  sf::Sprite spriteCloud1(textureCloud1);
  spriteCloud1.setPosition({0, 0});

  sf::Sprite spriteCloud2(textureCloud2);
  spriteCloud2.setPosition({0, 180});

  sf::Sprite spriteCloud3(textureCloud3);
  spriteCloud3.setPosition({0, 370});

  sf::Clock clock;
  while (window.isOpen()) {
    while (const std::optional maybeEvent = window.pollEvent()) {
      const sf::Event &event = *maybeEvent;

      eventHandler(event, window);
      keyPressedHandler(event, window);
    }

    sf::Time dt = clock.restart();

    window.clear(sf::Color::Black);
    if (!beeActive) {
      // How fast is the bee
      srand((int)time(0)); // seed rand
      beeSpeed = (rand() % 1000) + 500;

      // How high is the bee
      srand((int)time(0) * 10);
      float height = (rand() % 500) + 500;
      spriteBee.setPosition({1940, height});
      beeActive = true;
    } else {
      auto x = spriteBee.getPosition().x - (beeSpeed * dt.asSeconds());
      auto y = spriteBee.getPosition().y;
      spriteBee.setPosition({x, y});

      if (spriteBee.getPosition().x < -100) {
        beeActive = false;
      }
    }

    if (!cloudActive1) {
      // How fast is the cloud
      srand((int)time(0) * 10);
      cloudSpeed1 = (rand() % 250);

      // How high is the cloud
      srand((int)time(0) * 20);
      float height = (rand() % 300) - 150;
      spriteCloud1.setPosition({-200, height});
      cloudActive1 = true;
    } else {
      auto x = spriteCloud1.getPosition().x + (cloudSpeed1 * dt.asSeconds());
      auto y = spriteCloud1.getPosition().y;
      spriteCloud1.setPosition({x, y});

      if (spriteCloud1.getPosition().x > 1920) {
        cloudActive1 = false;
      }
    }

    if (!cloudActive2) {
      // How fast is the cloud
      srand((int)time(0) * 10);
      cloudSpeed2 = (rand() % 280);

      // How high is the cloud
      srand((int)time(0) * 30);
      float height = (rand() % 450) - 150;
      spriteCloud2.setPosition({-200, height});
      cloudActive2 = true;
    } else {
      auto x = spriteCloud2.getPosition().x + (cloudSpeed2 * dt.asSeconds());
      auto y = spriteCloud2.getPosition().y;
      spriteCloud2.setPosition({x, y});

      if (spriteCloud2.getPosition().x > 1920) {
        cloudActive2 = false;
      }
    }

    if (!cloudActive3) {
      // How fast is the cloud
      srand((int)time(0) * 10);
      cloudSpeed3 = (rand() % 200);

      // How high is the cloud
      srand((int)time(0) * 10);
      float height = (rand() % 550);
      spriteCloud3.setPosition({-200, height});
      cloudActive3 = true;
    } else {
      auto x = spriteCloud3.getPosition().x + (cloudSpeed3 * dt.asSeconds());
      auto y = spriteCloud3.getPosition().y;
      spriteCloud3.setPosition({x, y});

      if (spriteCloud3.getPosition().x > 1920) {
        cloudActive3 = false;
      }
    }

    // draw sprites here
    window.draw(spriteBackground);
    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);
    window.draw(spriteTree);
    window.draw(spriteBee);

    window.display();
  }
  return 0;
}
