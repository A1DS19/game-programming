#include "GameObject.h"
#include "Component.h"
#include "Graphics.h"
#include "Update.h"
#include <memory>

void GameObject::AddComponent(
    std::shared_ptr<Component> newComponent) noexcept {
  mComponents.push_back(newComponent);
}

void GameObject::Updatefn(float elapsedTime) noexcept {
  for (auto component : mComponents) {
    if (component->mIsUpdate) {
      std::static_pointer_cast<Update>(component)->Updatefn(elapsedTime);
    }
  }
}

void GameObject::Draw(sf::VertexArray &canvas) noexcept {
  for (auto component : mComponents) {
    if (component->mIsGraphics) {
      std::static_pointer_cast<Graphics>(component)->Draw(canvas);
    }
  }
}
