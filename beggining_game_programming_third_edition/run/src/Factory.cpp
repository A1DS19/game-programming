#include "Factory.h"
#include "GameObject.h"
#include <iostream>

Factory::Factory(sf::RenderWindow *window) : mWindow(window) {
  mTexture = new sf::Texture();

  if (!mTexture->loadFromFile("../assets/graphics/texture.png")) {
    std::cout << "texture not loaded" << std::endl;
    return;
  }
}

void Factory::LoadLevel(std::vector<GameObject> &gameObjects,
                        sf::VertexArray &canvas,
                        InputDispatcher &inputDispatcher) {}
