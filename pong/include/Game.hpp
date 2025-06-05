#pragma once

#include <SDL2/SDL.h>

class Game {
public:
  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  SDL_Window *mWindow = nullptr;
  SDL_Renderer *mRenderer = nullptr;
  bool mIsRunning = false;
};
