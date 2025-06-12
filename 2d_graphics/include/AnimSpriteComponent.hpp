#pragma once

#include <vector>

#include "SpriteComponent.hpp"

class Actor;
struct SDL_Texture;
class AnimSpriteComponent : public SpriteComponent {
public:
  AnimSpriteComponent(Actor *actor, int drawOrder = 100);
  // Update animation every frame
  void Update(float deltaTime) override;
  void SetAnimTextures(const std::vector<SDL_Texture *> &textures);

  float GetAnimFPS() const { return mAnimFPS; }
  void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
  // All textures in animation
  std::vector<SDL_Texture *> mAnimTextures;
  // Current displayed frame
  float mCurrFrame;
  // Animation frame rate, can be used to run animation at different fps
  float mAnimFPS;
};
