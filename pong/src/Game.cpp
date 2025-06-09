#include "Game.hpp"

Game::Game()
    : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true),
      mPaddleDir(0) {}

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

  mPaddlePos.x = 20.0f;
  mPaddlePos.y = SCREEN_H / 2.0f;
  mBallPos.x = SCREEN_W / 2.0f;
  mBallPos.y = SCREEN_H / 2.0f;
  mBallVel.x = -200.0f;
  mBallVel.y = 235.0f;

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

  // Update paddle direction
  mPaddleDir = 0;
  if (keyboardState[SDL_SCANCODE_W]) {
    mPaddleDir -= 1;
  }

  if (keyboardState[SDL_SCANCODE_S]) {
    mPaddleDir += 1;
  }
}

void Game::UpdateGame() {
  // wait for 16ms or clamp fps to 60
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  // difference in ticks from last frame converted to seconds
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

  // clamp max delta time
  if (deltaTime > 0.05f) {
    deltaTime = 0.05f;
  }

  // Update ticks count to next frame
  mTicksCount = SDL_GetTicks();

  if (mPaddleDir != 0) {
    // update y pos of paddle based on paddle direction * speed of 300.0f pixels
    // per second and delta time
    mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

    // clamp paddle to screen
    if (mPaddlePos.y < (mPaddleH / 2.0f + mThickness)) {
      mPaddlePos.y = mPaddleH / 2.0f + mThickness;
    } else if (mPaddlePos.y > (SCREEN_H - mPaddleH / 2.0f - mThickness)) {
      mPaddlePos.y = SCREEN_H - mPaddleH / 2.0f - mThickness;
    }
  }

  mBallPos.x += mBallVel.x * deltaTime;
  mBallPos.y += mBallVel.y * deltaTime;

  // bounce if needed
  // did ball intersect with paddle?
  float diff = mPaddlePos.y - mBallPos.y;
  diff = (diff > 0.0f) ? diff : -diff;

  // did ball collided with paddle
  if (diff <= mPaddleH / 2.0f && mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
      mBallVel.x < 0.0f) {
    mBallVel.x *= -1.0f;
    // check if wall went off screen
  } else if (mBallPos.x <= 0.0f) {
    mIsRunning = false;
    // did ball collided with rigth wall?
  } else if (mBallPos.x >= (SCREEN_W - mThickness) && mBallVel.x > 0.0f) {
    mBallVel.x *= -1.0f;
    // did ball collided with top wall
  } else if (mBallPos.y <= mThickness && mBallVel.y < 0.0f) {
    mBallVel.y *= -1.0f;
    // did ball collided with bottom wall
  } else if (mBallPos.y >= (SCREEN_H - mThickness) && mBallVel.y > 0.0f) {
    mBallVel.y *= -1.0f;
  }
}

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

  // Create left paddle
  SDL_Rect paddle{.x = static_cast<int>(mPaddlePos.x),
                  .y = static_cast<int>(mPaddlePos.y - mPaddleH / 2.0f),
                  .w = mThickness,
                  .h = static_cast<int>(mPaddleH)};
  SDL_RenderFillRect(mRenderer, &paddle);

  // Swap back and front buffers
  SDL_RenderPresent(mRenderer);
}
