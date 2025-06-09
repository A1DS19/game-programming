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
  static constexpr float mPaddleH = 100.0f;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  Vector2 mBallPos;
  Vector2 mPaddlePos;
  bool mIsRunning;
  Uint32 mTicksCount;
  int mPaddleDir;
  Vector2 mBallVel;
};
