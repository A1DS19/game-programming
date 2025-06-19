#pragma once

#include "Component.hpp"

class Actor;
struct SDL_Texture;
struct SDL_Renderer;

class SpriteComponent : public Component {
public:
  SpriteComponent(Actor *owner, int drawOrder = 100);
  ~SpriteComponent();

  virtual void Draw(SDL_Renderer *renderer);
  virtual void SetTexture(SDL_Texture *texture);

  int GetDrawOrder() const { return mDrawOrder; };
  int GetTexHeight() const { return mTextHeight; }
  int GetTexWidght() const { return mTexWidth; }

protected:
  SDL_Texture *mTexture;
  int mDrawOrder;
  int mTextHeight;
  int mTexWidth;
};
