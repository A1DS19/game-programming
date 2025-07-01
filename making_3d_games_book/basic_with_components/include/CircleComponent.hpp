#pragma once

#include "Component.hpp"

class Actor;
class Vector2;

class CircleComponent: public Component{
  public:
    CircleComponent(Actor *owner);

    void SetRadius(float radius){mRadius = radius;}
    float GetRadius() const;

    const Vector2& GetCenter() const;

  private:
    float mRadius;
};

bool Intersect(const CircleComponent &a, const CircleComponent &b);
