#include "SpriteComponent.hpp"

#include "Actor.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"

SpriteComponent::SpriteComponent(Actor *actor, int drawOrder)
    : Component(actor, drawOrder) {}

void SpriteComponent::SetTexture(SDL_Texture *texture) {
  mTexture = texture;
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexWidth);
}

void SpriteComponent::Draw(SDL_Renderer *renderer) {
  if (mTexture) {
    SDL_Rect r;
    // Scale the width/height by owner's scale
    r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    // Center the rectangle around the position of the owner
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

    // Draw (have to convert angle from radians to degrees, and clockwise to
    // counter)
    SDL_RenderCopyEx(renderer, mTexture, nullptr, &r,
                     -Math::ToDegrees(mOwner->GetRotation()), nullptr,
                     SDL_FLIP_NONE);
  }
}
