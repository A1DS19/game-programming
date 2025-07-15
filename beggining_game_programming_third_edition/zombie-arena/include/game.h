#pragma once

#include "SFML/Graphics.hpp"
#include "player.h"
#include <tuple>

using CloseHandler = std::function<void(const sf::Event::Closed &)>;
using KeyPressHandler = std::function<void(const sf::Event::KeyPressed &)>;
using Events = std::tuple<CloseHandler, KeyPressHandler>;

struct Game {
  enum class State { Paused, LevelingUp, GameOver, Playing };
  Game();
  void Run();
  void Update();
  void Draw();
  // using auto also works here instead of Events.
  Events Events();

  sf::RenderWindow mWindow;
  State mState;
  sf::Vector2f mResolution;
  sf::View mMainView;
  sf::Clock mClock;
  // How long has the PLAYING state been active
  sf::Time mGameTimeTotal;
  // Where is the mouse in
  // relation to world coordinates
  sf::Vector2f mMouseWorldPosition;
  // Where is the mouse in
  // relation to screen coordinates
  sf::Vector2f mMouseScreenPosition;
  // Create an instance of the Player class
  Player mPlayer;
  // The boundaries of the arena
  sf::IntRect mArena;
};
