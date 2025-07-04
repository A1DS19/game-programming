#include "Game.hpp"

#include <algorithm>
#include <vector>

#include "Actor.hpp"
#include "Asteroid.hpp"
#include "SDL_image.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include "Ship.hpp"
#include "SpriteComponent.hpp"

Game::Game()
    : mWindow{nullptr}, mRenderer{nullptr}, mIsRunning{true},
      mUpdatingActors{false} {}

bool Game::Initialize() {
  int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("asteroids", 100, 100, 1024, 768, 0);

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
    SDL_Log("Unable to initialize SDL_Image: %s", SDL_GetError());
    return false;
  }

  LoadData();
  mTicksCount = SDL_GetTicks();

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
  IMG_Quit();
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
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

  mUpdatingActors = true;
  for (auto actor : mActors) {
    actor->ProcessInput(keyboardState);
  }
  mUpdatingActors = false;
}

void Game::UpdateGame() {
  // Compute delta time
  // Basically waits 16ms after frame
  // Frame-rate limiting (≈60 FPS)
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  // Compute how long the actual frame took: (now − lastFrameTime) gives you ms,
  // divide by 1000 to get seconds.
  // Clamp it to a maximum (0.05 s) so if you ever hitch or pause (e.g. debugger
  // break), your physics/movement won’t suddenly jump by huge amounts.
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
  if (deltaTime > 0.05) {
    deltaTime = 0.05;
  }
  mTicksCount = SDL_GetTicks();

  // Update all actors.
  mUpdatingActors = true;
  for (auto actor : mActors) {
    actor->Update(deltaTime);
  }
  mUpdatingActors = false;

  // Move pending actors to mActors
  for (auto pending : mPendingActors) {
    mActors.emplace_back(pending);
  }
  mPendingActors.clear();

  // Add any dead actors to a temp vector
  std::vector<Actor *> deadActors;
  for (auto actor : mActors) {
    if (actor->GetState() == Actor::EDead) {
      deadActors.emplace_back(actor);
    }
  }

  // Delete dead actors (removes them from actors)
  for (auto actor : deadActors) {
    delete actor;
  }
}

void Game::GenerateOutput() {
  // Set background
  SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
  // Clear back buffer
  SDL_RenderClear(mRenderer);

  // Draw all sprite components
  for (auto sprite : mSprites) {
    sprite->Draw(mRenderer);
  }

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
  // Is it in pending actors?
  auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
  if (iter != mPendingActors.end()) {
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, mPendingActors.end() - 1);
    mPendingActors.pop_back();
  }

  // Is it in actors?
  iter = std::find(mActors.begin(), mActors.end(), actor);
  if (iter != mActors.end()) {
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, mActors.end() - 1);
    mActors.pop_back();
  }
}

void Game::AddSprite(SpriteComponent *sprite) {
  int drawOrder = sprite->GetDrawOrder();

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
  mSprites.erase(iter);
}

void Game::AddAsteroid(Asteroid *asteroid) {
  mAsteroids.emplace_back(asteroid);
}

void Game::RemoveAsteroid(Asteroid *asteroid) {
  auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), asteroid);
  if (iter != mAsteroids.end()) {
    mAsteroids.erase(iter);
  }
}

void Game::LoadData() {
  mShip = new Ship(this);
  mShip->SetPosition(Vector2(512.0f, 384.0f));
  mShip->SetRotation(Math::PiOver2);

  const int numAsteroids = 20;
  for (int i = 0; i < numAsteroids; i++) {
    new Asteroid(this);
  }
}

void Game::UnloadData() {
  while (!mActors.empty()) {
    delete mActors.back();
  }

  for (auto i : mTextures) {
    SDL_DestroyTexture(i.second);
  }
  mTextures.clear();
}

SDL_Texture *Game::GetTexture(const std::string &filename) {
  SDL_Texture *tex = nullptr;

  auto iter = mTextures.find(filename);
  if (iter != mTextures.end()) {
    tex = iter->second;
  } else {
    // Load from file.
    SDL_Surface *surf = IMG_Load(filename.c_str());
    if (!surf) {
      SDL_Log("Failed to load texture file: %s", filename.c_str());
      return nullptr;
    }

    // Create texture from surface.
    tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
      SDL_Log("Failed to convert surface to texture: %s", filename.c_str());
      return nullptr;
    }

    mTextures.emplace(filename.c_str(), tex);
  }

  return tex;
}
