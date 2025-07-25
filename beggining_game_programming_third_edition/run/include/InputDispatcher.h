#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

class InputReciever;
class InputDispatcher {
private:
  sf::RenderWindow *mWindow;
  std::vector<InputReciever *> mInputRecievers;

public:
  InputDispatcher(sf::RenderWindow &window);
  void DispatchInputEvents();
  void RegisterNewInputReciever(InputReciever &ir);
};
