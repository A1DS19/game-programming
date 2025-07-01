#pragma once
#include "Component.hpp"

struct SDL_Renderer;
struct SDL_Texture;
class Actor;
class SpriteComponent : public Component {
public:
  SpriteComponent(Actor *actor, int drawOrder = 100);
  virtual ~SpriteComponent();
  virtual void Draw(SDL_Renderer *renderer);
  virtual void SetTexture(SDL_Texture *texture);

  int GetDrawOrder() const { return mDrawOrder; }
  int GetTexWidth() const { return mTexWidth; }
  int GetTexHeight() const { return mTexHeight; }

protected:
  // Texture to draw
  SDL_Texture *mTexture;
  // Draw order(for painters algorithm)
  int mDrawOrder;
  // W/H for texture
  int mTexWidth;
  int mTexHeight;
};
