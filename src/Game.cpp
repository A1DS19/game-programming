#include "Game.hpp"

Game::Game() {}

bool Game::Initialize() {
  int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, 1024, 768, 0);

  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mIsRunning = true;

  return true;
}

void Game::RunLoop() {
  while (mIsRunning) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::Shutdown() {
  SDL_DestroyWindow(mWindow);
  mWindow = nullptr;
  SDL_Quit();
  mIsRunning = false;
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      mIsRunning = false;
    }
  }
}

void Game::UpdateGame() {}

void Game::GenerateOutput() {
  // 60 FPS
  SDL_Delay(16);
}
