#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_render.h"
#include "SDL_stdinc.h"

class Actor;
class SpriteComponent;
class Ship;
class Game {
public:
  Game();
  bool Initialize();
  void RunLoop();
  void Shutdown();
  void AddActor(Actor *actor);
  void RemoveActor(Actor *actor);
  void AddSprite(SpriteComponent *sprite);
  void RemoveSprite(SpriteComponent *sprite);
  SDL_Texture *GetTexture(const std::string &filename);

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
  void UnloadData();
  void LoadData();

  SDL_Window *mWindow = nullptr;
  SDL_Renderer *mRenderer = nullptr;
  bool mIsRunning = false;
  std::vector<Actor *> mActors;
  std::vector<Actor *> mPendingActors;
  bool mUpdatingActors;
  Uint32 mTicksCount;
  std::unordered_map<std::string, SDL_Texture *> mTextures;
  std::vector<SpriteComponent *> mSprites;
  Ship *mShip;
};
