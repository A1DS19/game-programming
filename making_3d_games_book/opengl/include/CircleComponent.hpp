#pragma once

#include "Component.hpp"

class Actor;
class Vector3;

class CircleComponent : public Component {
public:
  CircleComponent(Actor *owner);

  void SetRadius(float radius) { mRadius = radius; }
  float GetRadius() const;

  const Vector3 &GetCenter() const;

private:
  float mRadius;
};

bool Intersect(const CircleComponent &a, const CircleComponent &b);
