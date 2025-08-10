#include "MoveComponent.hpp"

#include "Actor.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "Math.hpp"

MoveComponent::MoveComponent(Actor *owner, int updateOrder)
    : Component(owner, updateOrder), mAngularSpeed(0.0f), mForwardSpeed(0.0f) {}

void MoveComponent::Update(float deltaTime) {
  // Handle rotation if angular speed is non-zero
  if (!Math::NearZero(mAngularSpeed)) {
    // Get current rotation quaternion
    Quaternion rot = mOwner->GetRotation();
    // Calculate rotation angle for this frame
    float angle = mAngularSpeed * deltaTime;
    // Create incremental rotation quaternion around Z-axis (unit quaternion
    // example)
    Quaternion inc(Vector3::UnitZ, angle);
    // Concatenate rotations: apply increment to current rotation
    rot = Quaternion::Concatenate(rot, inc);
    // Update actor's rotation
    mOwner->SetRotation(rot);
  }

  // Handle forward movement if forward speed is non-zero
  if (!Math::NearZero(mForwardSpeed)) {
    // Get current position
    Vector3 pos = mOwner->GetPosition();
    // Move forward by speed * deltaTime in the forward direction
    pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
    // Update actor's position
    mOwner->SetPosition(pos);
  }
}
