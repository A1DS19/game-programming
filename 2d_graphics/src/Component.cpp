#include "Component.hpp"

#include "Actor.hpp"

Component::Component(Actor *owner, int updateOrder)
    : mOwner{owner}, mUpdateOrder{updateOrder} {}

void Component::Update(float deltaTime) {}
