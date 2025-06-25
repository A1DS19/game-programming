#pragma once

// Give behavior to each state from main AIState.
class AIComponent;
class AIState {
public:
  AIState(AIComponent *owner) : mOwner(owner) {};

  // state specific behavior
  virtual void Update(float deltaTime) = 0;
  virtual void OnEnter() = 0;
  virtual void OnExit() = 0;

  // getters/setters
  virtual const char *GetName() const = 0;

protected:
  AIComponent *mOwner;
};

class AIPatrol : public AIState {
public:
  AIPatrol(AIComponent *owner) : AIState(owner) {}

  // Override with behaviors
  void Update(float deltaTime) override;
  void OnEnter() override;
  void OnExit() override;

  const char *GetName() const override { return "Patrol"; }
};

class AIDeath : public AIState {
public:
  AIDeath(class AIComponent *owner) : AIState(owner) {}

  void Update(float deltaTime) override;
  void OnEnter() override;
  void OnExit() override;

  const char *GetName() const override { return "Death"; }
};

class AIAttack : public AIState {
public:
  AIAttack(class AIComponent *owner) : AIState(owner) {}

  void Update(float deltaTime) override;
  void OnEnter() override;
  void OnExit() override;

  const char *GetName() const override { return "Attack"; }
};
