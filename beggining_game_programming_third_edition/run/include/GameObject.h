#pragma once

#include <memory>
#include <vector>

#include "SFML/Graphics.hpp"

class Component;
class GameObject {
private:
  std::vector<std::shared_ptr<Component>> mComponents;

public:
  void AddComponent(std::shared_ptr<Component> newComponent) noexcept;
  void Updatefn(float elapsedTime) noexcept;
  void Draw(sf::VertexArray &canvas) noexcept;
};
