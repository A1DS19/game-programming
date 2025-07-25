#pragma once

#include "Component.h"
#include "SFML/Graphics.hpp"
#include <memory>

class Update;
class Graphics : public Component {
public:
  Graphics();
  virtual void Assemble(sf::VertexArray &canvas,
                        std::shared_ptr<Update> genericUpdate,
                        sf::IntRect &textCoords) = 0;
  virtual void Draw(sf::VertexArray &canvas) = 0;
};
