#include "events.hpp"
#include "sprites.hpp"

int main() {
  constexpr auto screenDims = sf::Vector2u{1920, 1080};
  sf::RenderWindow window(sf::VideoMode(screenDims), "Timber");

  window.setVerticalSyncEnabled(true);

  sf::Texture textureBackground;
  if (!loadTexture(textureBackground, "../assets/graphics/background.png")) {
    return 1;
  }

  sf::Texture textureTree;
  if (!loadTexture(textureTree, "../assets/graphics/tree.png")) {
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

  while (window.isOpen()) {
    while (const std::optional maybeEvent = window.pollEvent()) {
      const sf::Event &event = *maybeEvent;

      eventHandler(event, window);
      keyPressedHandler(event, window);
    }

    window.clear(sf::Color::Black);

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
