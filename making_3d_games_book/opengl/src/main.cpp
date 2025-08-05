#include "Game.hpp"

int main() {
  Game game;
  auto success = game.Initialize();

  if (success) {
    game.RunLoop();
  }

  game.Shutdown();
  return 0;
}
