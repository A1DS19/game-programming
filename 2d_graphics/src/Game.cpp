#include "Game.hpp"

#include <algorithm>
#include <vector>

#include "Actor.hpp"
#include "SDL_image.h"
#include "SDL_timer.h"
#include "SpriteComponent.hpp"

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

  mRenderer = SDL_CreateRenderer(
      mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!mRenderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
  }

  if (IMG_Init(IMG_INIT_PNG) == 0) {
    SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
    return false;
  }

  LoadData();

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
  UnloadData();
  SDL_DestroyWindow(mWindow);
  mWindow = nullptr;
  SDL_DestroyRenderer(mRenderer);
  mRenderer = nullptr;
  SDL_Quit();
  mIsRunning = false;
}

void Game::UnloadData() {
  while (!mActors.empty()) {
    delete mActors.back(); // destroy the object
    mActors.pop_back();    // remove the pointer from the vector
  }
}

void Game::LoadData() {}

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

void Game::UpdateGame() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
  if (deltaTime > 0.05f) {
    deltaTime = 0.05f;
  }
  mTicksCount = SDL_GetTicks();

  mUpdatingActors = true;
  for (auto actor : mActors) {
    actor->Update(deltaTime);
  }
  mUpdatingActors = false;

  for (auto pending : mPendingActors) {
    mActors.emplace_back(pending);
  }
  // delete all elements from pending actors
  mPendingActors.clear();

  std::vector<Actor *> deadActors;
  for (auto actor : mActors) {
    if (actor->GetState() == Actor::State::EDead) {
      deadActors.emplace_back(actor);
    }
  }

  for (auto actor : deadActors) {
    delete actor;
  }
}

void Game::GenerateOutput() {
  // Set background
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
  // Clear back buffer
  SDL_RenderClear(mRenderer);
  // Swap back and front buffers
  SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor *actor) {
  if (mUpdatingActors) {
    mPendingActors.emplace_back(actor);
  } else {
    mActors.emplace_back(actor);
  }
}

void Game::RemoveActor(Actor *actor) {
  auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
  if (iter != mPendingActors.end()) {
    // This is an O(1) removal: we swap the target with the last element
    // and pop it, so we don’t have to shift every element down one slot.
    std::iter_swap(iter, mPendingActors.end() - 1);
    mPendingActors.pop_back();
  }

  iter = std::find(mActors.begin(), mActors.end(), actor);
  if (iter != mActors.end()) {
    // This is an O(1) removal: we swap the target with the last element
    // and pop it, so we don’t have to shift every element down one slot.
    std::iter_swap(iter, mActors.end() - 1);
    mActors.pop_back();
  }
}

void Game::AddSprite(SpriteComponent *sprite) {
  auto drawOrder = sprite->GetDrawOrder();
  auto iter = mSprites.begin();

  for (; iter != mSprites.end(); ++iter) {
    if (drawOrder < (*iter)->GetDrawOrder()) {
      break;
    }
  }

  mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent *sprite) {
  auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
  if (iter != mSprites.end()) {
    mSprites.erase(iter);
  }
}
