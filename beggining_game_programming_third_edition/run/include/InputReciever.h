#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

class InputReciever {
private:
  std::vector<sf::Event> mEvents;

public:
  void AddEvent(const sf::Event &event) noexcept;
  const std::vector<sf::Event> &GetEvents() const noexcept { return mEvents; }
  void ClearEvents() noexcept;
};
