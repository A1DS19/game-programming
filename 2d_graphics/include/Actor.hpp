#pragma once

#include <vector>

#include "Math.hpp"

/*
Rule of thumb:
If your class has any virtual functions, give it a virtual destructor.
*/

class Actor {
public:
  // Track state of actor
  enum State { EActive, EPaused, EDead };

  Actor(class Game *game);
  virtual ~Actor();

  // Update function called from Game
  void Update(float deltaTime);
  // Updates all components attached to the actor
  void UpdateComponents(float deltaTime);
  // Any specific actor update code
  virtual void UpdateActor(float deltaTime);
  void AddComponent(class Component *component);
  void RemoveComponent(class Component *component);

  State GetState() const { return mState; }
  void SetState(State state) { mState = state; }
  Game *GetGame() { return mGame; }

  const Vector2 &GetPosition() const { return mPosition; }
  void SetPosition(const Vector2 &pos) { mPosition = pos; }
  float GetScale() const { return mScale; }
  void SetScale(float scale) { mScale = scale; }
  float GetRotation() const { return mRotation; }
  void SetRotation(float rotation) { mRotation = rotation; }

private:
  State mState;
  Vector2 mPosition;
  // Uniform scale (1.0f to 100.0f)
  float mScale;
  float mRotation;
  std::vector<class Component *> mComponents;
  class Game *mGame;
};
