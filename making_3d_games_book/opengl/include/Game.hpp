#pragma once

#include <SDL2/SDL.h>

#include "Renderer.hpp"
#include <vector>

#include "SDL_stdinc.h"

class Actor;
class Renderer;
class CameraActor;
class Game {
public:
  static constexpr float SCREEN_WIDTH = 1024.0f;
  static constexpr float SCREEN_HEIGHT = 768.0f;

  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();

  void AddActor(Actor *actor);
  void RemoveActor(Actor *actor);

  class Renderer *GetRenderer() { return mRenderer; }

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
  void LoadData();
  void UnloadData();

  // All the actors in the game
  std::vector<class Actor *> mActors;
  // Any pending actors
  std::vector<class Actor *> mPendingActors;

  class Renderer *mRenderer;

  Uint32 mTicksCount;
  bool mIsRunning;
  // Track if we're updating actors right now
  bool mUpdatingActors;

  // Game-specific code
  CameraActor *mCameraActor;
};
