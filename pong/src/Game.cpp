#include "Game.hpp"

Game::Game() {
  mBallPos = {.x = SCREEN_W / 2, .y = SCREEN_H / 2};
  mPaddlePos = {.x = mThickness + 10, .y = SCREEN_H / 2};
}

bool Game::Initialize() {
  int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);

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
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
  // Clear back buffer
  SDL_RenderClear(mRenderer);

  // Set background again
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  // Create walls
  SDL_Rect topWall{.x = 0, .y = 0, .w = SCREEN_W, .h = mThickness};
  SDL_RenderFillRect(mRenderer, &topWall);

  SDL_Rect bottomWall{
      .x = 0, .y = SCREEN_H - mThickness, .w = SCREEN_W, .h = mThickness};
  SDL_RenderFillRect(mRenderer, &bottomWall);

  SDL_Rect leftWall{.x = 0, .y = 0, .w = mThickness, .h = SCREEN_H};
  SDL_RenderFillRect(mRenderer, &leftWall);

  SDL_Rect rightWall{
      .x = SCREEN_W - mThickness, .y = 0, .w = mThickness, .h = SCREEN_H};
  SDL_RenderFillRect(mRenderer, &rightWall);

  // Create ball
  SDL_Rect ball{.x = static_cast<int>(mBallPos.x - mThickness / 2),
                .y = static_cast<int>(mBallPos.y - mThickness / 2),
                .w = mThickness,
                .h = mThickness};
  SDL_RenderFillRect(mRenderer, &ball);

  // Create paddle
  SDL_Rect paddle{
      .x = static_cast<int>(mPaddlePos.x),
      .y = static_cast<int>(mPaddlePos.y - (mThickness / 2) - (mPaddleW / 2)),
      .w = mThickness,
      .h = mPaddleW};
  SDL_RenderFillRect(mRenderer, &paddle);

  // Swap back and front buffers
  SDL_RenderPresent(mRenderer);
}
