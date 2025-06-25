#pragma once

#include <cstdint>

class Actor;
class Component {
public:
  // Components are owned by actor.
  // The lower the update order the earlier the components updates.
  Component(Actor *owner, int updateOrder = 100);
  virtual ~Component();
  // Update this component by delta time.
  virtual void Update(float deltaTime);
  // Process input for this component.
  virtual void ProcessInput(const uint8_t *keyState) {}

  // Getters/setters
  int GetUpdateOrder() const { return mUpdateOrder; }

protected:
  // Owning actor.
  Actor *mOwner;
  // Update order of component.
  int mUpdateOrder;
};
