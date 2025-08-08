#pragma once

#include "Component.hpp"

class Actor;
struct SDL_Renderer;
class Texture;
class Shader;

class SpriteComponent : public Component {
public:
  SpriteComponent(Actor *owner, int drawOrder = 100);
  ~SpriteComponent();

  virtual void Draw(Shader *shader);
  virtual void SetTexture(Texture *texture);

  int GetDrawOrder() const { return mDrawOrder; };
  int GetTexHeight() const { return mTextHeight; }
  int GetTexWidght() const { return mTexWidth; }

protected:
  Texture *mTexture;
  int mDrawOrder;
  int mTextHeight;
  int mTexWidth;
};
