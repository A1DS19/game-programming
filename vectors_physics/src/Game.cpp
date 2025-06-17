#include "Game.hpp"

Game::Game() {}

bool Game::Initialize() {
  int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("asteroids", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, 1024, 768, 0);

  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(
      mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!mRenderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
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
  SDL_DestroyRenderer(mRenderer);
  mRenderer = nullptr;
  SDL_Quit();
  mIsRunning = false;
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      mIsRunning = false;
      break;
    }
  }

  // Grabs the entire keyboard state
  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
  if (keyboardState[SDL_SCANCODE_ESCAPE]) {
    mIsRunning = false;
  }
}

void Game::UpdateGame() {}

void Game::GenerateOutput() {
  // Set background
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
  // Clear back buffer
  SDL_RenderClear(mRenderer);
  // Swap back and front buffers
  SDL_RenderPresent(mRenderer);
}
