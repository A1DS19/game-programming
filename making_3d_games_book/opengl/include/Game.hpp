#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_stdinc.h"

class Actor;
class SpriteComponent;
class Asteroid;
class Ship;
class VertexArray;
class Shader;
class Texture;
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

  void AddSprite(SpriteComponent *sprite);
  void RemoveSprite(SpriteComponent *sprite);

  Texture *GetTexture(const std::string &filename);

  // game specific
  void AddAsteroid(Asteroid *asteroid);
  void RemoveAsteroid(Asteroid *asteroid);
  std::vector<Asteroid *> &GetAsteroids() { return mAsteroids; }

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
  void LoadData();
  void UnloadData();
  void CreateSpriteVerts();
  bool LoadShaders();

  std::unordered_map<std::string, Texture *> mTextures;
  std::vector<Actor *> mActors;
  std::vector<Actor *> mPendingActors;
  bool mUpdatingActors;
  std::vector<SpriteComponent *> mSprites;
  VertexArray *mSpriteVerts;
  Shader *mSpriteShader;

  SDL_Window *mWindow;
  SDL_GLContext mContext;
  SDL_Renderer *mRenderer;
  Uint32 mTicksCount;
  bool mIsRunning;

  // game specific
  Ship *mShip;
  std::vector<Asteroid *> mAsteroids;
};
