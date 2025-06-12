#include "AnimSpriteComponent.hpp"

#include "SpriteComponent.hpp"

void AnimSpriteComponent::Update(float deltaTime) {
  SpriteComponent::Update(deltaTime);
  if (mAnimTextures.size() > 0) {
    // Update the current frame based on fps and delta time
    mCurrFrame += mAnimFPS * deltaTime;

    while (mCurrFrame >= mAnimTextures.size()) {
      mCurrFrame -= mAnimTextures.size();
    }

    SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
  }
}
