#pragma once

#include "Component.hpp"
#include <cstddef>
#include <strings.h>

class Actor;
class Shader;
class Mesh;
class MeshComponent : public Component {
public:
  MeshComponent(Actor *owner);
  ~MeshComponent();
  // Draw this mesh
  virtual void Draw(Shader *shader);
  // Set the mesh/texture index
  virtual void SetMesh(Mesh *mesh) { mMesh = mesh; }
  void SetTextureIndex(size_t index) { mTextureIndex = index; }

private:
  Mesh *mMesh;
  size_t mTextureIndex;
};
