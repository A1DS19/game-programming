#include "SpriteComponent.hpp"

#include "Actor.hpp"
#include "Component.hpp"
#include "GL/glew.h"
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
  // Scale the quad by the width/height of texture
  Matrix4 scaleMat = Matrix4::CreateScale(
      static_cast<float>(mTexWidth), static_cast<float>(mTextHeight), 1.0f);

  Matrix4 world = scaleMat * mOwner->GetWorldTransform();

  // Since all sprites use the same shader/vertices,
  // the game first sets them active before any sprite draws

  // Set world transform
  shader->SetMatrixUniform("uWorldTransform", world);

  // Set current texture
  // mTexture->SetActive();

  // Draw quad
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void SpriteComponent::SetTexture(SDL_Texture *texture) {
  mTexture = texture;

  // Set w/h
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTextHeight);
}
