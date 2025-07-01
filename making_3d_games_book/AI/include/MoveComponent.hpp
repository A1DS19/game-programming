#pragma once

#include "Component.hpp"

class Actor;
class MoveComponent : public Component {
public:
  MoveComponent(Actor *owner, int updateOrder = 10);

  void Update(float deltaTime) override;

  float GetAngularSpeed() const { return mAngularSpeed; }
  float GetForwardSpeed() const { return mForwardSpeed; }
  void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
  void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
  // Controls rotation(radians/second)
  float mAngularSpeed;
  // Controls forward movement(units/second)
  float mForwardSpeed;
};
