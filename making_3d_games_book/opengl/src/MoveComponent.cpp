#include "MoveComponent.hpp"

#include "Actor.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "Math.hpp"

MoveComponent::MoveComponent(Actor *owner, int updateOrder)
    : Component(owner, updateOrder), mAngularSpeed(0.0f), mForwardSpeed(0.0f) {}

void MoveComponent::Update(float deltaTime) {
  if (!Math::NearZero(mAngularSpeed)) {
    float rot = mOwner->GetRotation();
    rot += mAngularSpeed * deltaTime;
    mOwner->SetRotation(rot);
  }

  if (!Math::NearZero(mForwardSpeed)) {
    Vector2 pos = mOwner->GetPosition();
    pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

    // (Screen wrapping code only for asteroids)
    if (pos.x < 0.0f) {
      pos.x = Game::SCREEN_WIDTH - 2.0f;
    } else if (pos.x > Game::SCREEN_WIDTH) {
      pos.x = 2.0f;
    }

    if (pos.y < 0.0f) {
      pos.y = Game::SCREEN_HEIGHT - 2.0f;
    } else if (pos.y > Game::SCREEN_HEIGHT) {
      pos.y = 2.0f;
    }

    mOwner->SetPosition(pos);
  }
}
