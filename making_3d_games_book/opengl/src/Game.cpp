#include "Game.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include "Actor.hpp"
#include "GL/glew.h"
#include "MeshComponent.hpp"
#include "Renderer.hpp"
#include "SDL_log.h"
#include "SDL_timer.h"
#include "SpriteComponent.hpp"

Game::Game() : mRenderer{nullptr}, mIsRunning{true}, mUpdatingActors{false} {}

bool Game::Initialize() {
  // Try to initialize with audio first
  int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (sdlResult != 0) {
    SDL_Log("Failed to initialize with audio: %s", SDL_GetError());
    SDL_Log("Trying without audio...");

    // Fall back to video only
    sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
      SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
      return false;
    }
    SDL_Log("SDL initialized without audio");
  } else {
    SDL_Log("SDL initialized with audio");
  }

  mRenderer = new Renderer(this);
  if (!mRenderer->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    SDL_Log("Failed to initialize renderer");
    delete mRenderer;
    mRenderer = nullptr;
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
  if (mRenderer) {
    mRenderer->Shutdown();
  }
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
    pending->ComputeWorldTransform();
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

void Game::GenerateOutput() { mRenderer->Draw(); }

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

void Game::LoadData() {
  // Create actors
  Actor *a = new Actor(this);
  a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
  a->SetScale(100.0f);
  Quaternion q(Vector3::UnitY, -Math::PiOver2);
  q = Quaternion::Concatenate(
      q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
  a->SetRotation(q);
  MeshComponent *mc = new MeshComponent(a);
  mc->SetMesh(mRenderer->GetMesh("../assets/Cube.gpmesh"));

  a = new Actor(this);
  a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
  a->SetScale(3.0f);
  mc = new MeshComponent(a);
  mc->SetMesh(mRenderer->GetMesh("../assets/Sphere.gpmesh"));
}

void Game::UnloadData() {
  while (!mActors.empty()) {
    delete mActors.back();
  }

  if (mRenderer) {
    mRenderer->UnloadData();
  }
}
