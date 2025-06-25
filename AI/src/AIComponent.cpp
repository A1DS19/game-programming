#include "AIComponent.hpp"

#include "AIState.hpp"
#include "Actor.hpp"
#include "Component.hpp"
#include "SDL_log.h"

AIComponent::AIComponent(Actor *owner)
    : Component(owner), mCurrentState(nullptr) {}

void AIComponent::Update(float deltaTime) {
  if (mCurrentState) {
    mCurrentState->Update(deltaTime);
  }
}

void AIComponent::ChangeState(const std::string &name) {
  // Exit of current state
  if (mCurrentState) {
    mCurrentState->OnExit();
  }

  auto iter = mStateMap.find(name);
  if (iter != mStateMap.end()) {
    mCurrentState = iter->second;
    // Enter new state
    mCurrentState->OnEnter();
  } else {
    SDL_Log("Could not find AIState %s in state map", name.c_str());
    mCurrentState = nullptr;
  }
}

void AIComponent::RegisterState(AIState *state) {
  mStateMap.emplace(state->GetName(), state);
}
