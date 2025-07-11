#pragma once

#include "SFML/Graphics.hpp"
#include <tuple>

using CloseHandler = std::function<void(const sf::Event::Closed &)>;
using KeyPressHandler = std::function<void(const sf::Event::KeyPressed &)>;
using Events = std::tuple<CloseHandler, KeyPressHandler>;

struct Game {
  static constexpr sf::Vector2u ScreenDims{1920, 1080};
  Game();
  void Run();
  void Update();
  void Draw();
  // using auto also works here instead of Events.
  Events Events();

  sf::RenderWindow mWindow;
};
