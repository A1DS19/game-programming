#pragma once

#include <cstdint>
#include <vector>

#include "Math.hpp"

class Game;
class Component;
class Matrix4;
class Actor {
public:
  enum State { EActive, EPaused, EDead };

  Actor(Game *game);
  virtual ~Actor();

  // Update member function called from game.
  void Update(float deltaTime);
  // Updates all components attached to the actor.
  void UpdateComponents(float deltaTime);
  // Updates actor-specific code.
  virtual void UpdateActor(float deltaTime);

  // Process input function called from game.
  void ProcessInput(const uint8_t *keyState);
  // Actor specific input code.
  virtual void ActorInput(const uint8_t *keyState);

  // Getters/setters.
  const Vector3 &GetPosition() const { return mPosition; }
  void SetPosition(const Vector3 &pos) { mPosition = pos; }
  float GetScale() const { return mScale; }
  void SetScale(float scale) { mScale = scale; }
  const Quaternion &GetRotation() const { return mRotation; }
  void SetRotation(Quaternion rotation) { mRotation = rotation; }
  Vector3 GetForward() const {
    return Vector3::Transform(Vector3::UnitX, mRotation);
  }
  void ComputeWorldTransform();
  State GetState() const { return mState; }
  void SetState(State state) { mState = state; }
  Game *GetGame() { return mGame; }
  const Matrix4 &GetWorldTransform() const { return mWorldTransform; }

  // Add/remove components.
  void AddComponent(Component *component);
  void RemoveComponent(Component *component);

private:
  // Actors state.
  State mState;

  // Transform
  Vector3 mPosition;
  float mScale;
  Quaternion mRotation;
  Matrix4 mWorldTransform;
  bool mRecomputeWorldTransform;

  std::vector<Component *> mComponents;
  Game *mGame;
};
