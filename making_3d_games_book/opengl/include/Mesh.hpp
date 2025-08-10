#pragma once

#include <cstddef>
#include <string>
#include <vector>

class Renderer;
class VertexArray;
class Texture;

class Mesh {

public:
  Mesh();
  ~Mesh();
  bool Load(const std::string &fileName, Renderer *renderer);
  void Unload();
  VertexArray *GetVertexArray() { return mVertexArray; }
  Texture *GetTexture(size_t index);
  const std::string &GetShaderName() const { return mShaderName; }
  float GetRadius() const { return mRadius; }
  float GetSpecPower() const { return mSpecPower; }

private:
  std::vector<Texture *> mTextures;
  VertexArray *mVertexArray;
  std::string mShaderName;
  float mRadius;
  float mSpecPower;
};
