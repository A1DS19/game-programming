#include "Actor.hpp"

#include <cstdint>

#include "Component.hpp"
#include "Game.hpp"

Actor::Actor(Game *game)
    : mState(EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f),
      mGame(game), mRecomputeWorldTransform(true) {
  mGame->AddActor(this);
}

Actor::~Actor() {
  mGame->RemoveActor(this);

  // Delete all components associated with actor.
  // Requires this style of loop.
  while (!mComponents.empty()) {
    delete mComponents.back();
  }
}

void Actor::Update(float deltaTime) {
  if (mState == EActive) {
    UpdateComponents(deltaTime);
    UpdateActor(deltaTime);
  }
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::UpdateComponents(float deltaTime) {
  for (auto comp : mComponents) {
    comp->Update(deltaTime);
  }
}

void Actor::ProcessInput(const uint8_t *keyState) {
  if (mState == EActive) {
    for (auto comp : mComponents) {
      comp->ProcessInput(keyState);
    }

    ActorInput(keyState);
  }
}

void Actor::ActorInput(const uint8_t *keyState) {}

void Actor::AddComponent(Component *component) {
  // Find the insertion point in the sorted vector.
  // The element with a order higher than me.
  int myOrder = component->GetUpdateOrder();
  auto iter = mComponents.begin();
  for (; iter != mComponents.end(); ++iter) {
    if (myOrder < (*iter)->GetUpdateOrder()) {
      break;
    }
  }

  // Inserts element before of iterator
  mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component *component) {
  auto iter = std::find(mComponents.begin(), mComponents.end(), component);
  if (iter != mComponents.end()) {
    mComponents.erase(iter);
  }
}

void Actor::ComputeWorldTransform() {
  if (mRecomputeWorldTransform) {
    mRecomputeWorldTransform = false;
    // Scale.
    mWorldTransform = Matrix4::CreateScale(mScale);
    // Rotate.
    mWorldTransform *= Matrix4::CreateRotationZ(mRotation);
    // Translate.
    mWorldTransform *=
        Matrix4::CreateTranslation(Vector3(mPosition.x, mPosition.y, 0.0f));

    // Inform components world updated.
    for (auto comp : mComponents) {
      // comp->OnUpdateWorldTransform();
    }
  }
}
