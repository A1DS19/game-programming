#pragma once

/*
Rule of thumb:
If your class has any virtual functions, give it a virtual destructor.
*/

class Actor;
class Component {
public:
  // The lower the update order is then it will update quicker
  Component(Actor *owner, int updateOrder = 100);
  virtual ~Component();

  // Update the component by its deltaTime
  virtual void Update(float deltaTime);
  int GetUpdateOrder() const { return mUpdateOrder; }

protected:
  Actor *mOwner;
  int mUpdateOrder;
};
