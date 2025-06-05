#pragma once

#include <SDL2/SDL.h>

class Game {
public:
  static constexpr int SCREEN_W = 1024;
  static constexpr int SCREEN_H = 768;
  struct Vector2 {
    float x;
    float y;
  };

  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

private:
  static constexpr int mThickness = 15;
  static constexpr int mPaddleW = 100;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  SDL_Window *mWindow = nullptr;
  SDL_Renderer *mRenderer = nullptr;
  Vector2 mBallPos;
  Vector2 mPaddlePos;
  bool mIsRunning = false;
};
