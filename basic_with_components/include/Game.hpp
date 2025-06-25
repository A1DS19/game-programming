#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_stdinc.h"

struct SDL_Texture;
class Actor;
class SpriteComponent;
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
  void LoadData();
  void UnloadData();

  std::unordered_map<std::string, SDL_Texture *> mTextures;
  std::vector<Actor *> mActors;
  std::vector<Actor *> mPendingActors;
  bool mUpdatingActors;
  std::vector<SpriteComponent *> mSprites;

  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  Uint32 mTicksCount;
  bool mIsRunning;
};
