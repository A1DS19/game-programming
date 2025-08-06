#include "SpriteComponent.hpp"

#include "Actor.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Shader.hpp"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
    : Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0),
      mTextHeight(0) {
  mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() { mOwner->GetGame()->RemoveSprite(this); }

void SpriteComponent::Draw(Shader *shader) {
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  // if (mTexture) {
  //   SDL_Rect r;
  //   // Scales the texture depending on the owners scale.
  //   r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
  //   r.h = static_cast<int>(mTextHeight * mOwner->GetScale());

  //   // Center rect around position of owner.
  //   r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
  //   r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

  //   // Draw.
  //   SDL_RenderCopyEx(renderer, mTexture, nullptr, &r,
  //                    -Math::ToDegrees(mOwner->GetRotation()), nullptr,
  //                    SDL_FLIP_NONE);
  // }
}

void SpriteComponent::SetTexture(SDL_Texture *texture) {
  mTexture = texture;

  // Set w/h
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTextHeight);
}
