#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

class GameObject;
class InputDispatcher;
class Factory {
private:
  sf::RenderWindow *mWindow;

public:
  Factory(sf::RenderWindow *window);
  void LoadLevel(std::vector<GameObject> &gameObjects, sf::VertexArray &canvas,
                 InputDispatcher &inputDispatcher);
  sf::Texture *mTexture;
};
