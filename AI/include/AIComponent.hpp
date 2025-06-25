#pragma once

#include <string>
#include <unordered_map>

#include "Component.hpp"

class Actor;
class AIState;
class AIComponent : public Component {
public:
  AIComponent(Actor *owner);

  void Update(float deltaTime) override;
  void ChangeState(const std::string &state);

  // Add new state to map
  void RegisterState(AIState *state);

private:
  // Maps name of state to AIState instance
  std::unordered_map<std::string, AIState *> mStateMap;
  // Current state we are in
  AIState *mCurrentState;
};
