#include "MeshComponent.hpp"
#include "Actor.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "SDL_log.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

MeshComponent::MeshComponent(Actor *owner)
    : Component(owner), mMesh(nullptr), mTextureIndex(0) {
  mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() {
  mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader *shader) {
  if (mMesh) {
    // Set the world transform
    shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    // Set active Texture
    auto *texture = mMesh->GetTexture(mTextureIndex);
    if (texture) {
      texture->SetActive();
    }

    auto *va = mMesh->GetVertexArray();
    va->SetActive();

    glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
  }
}
