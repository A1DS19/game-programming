#include "textureHolder.h"
#include <cassert>
#include <iostream>

static TextureHolder s_textureHolder;

// Static singleton pointer, initially null.
TextureHolder *TextureHolder::msInstance = nullptr;

// Constructor: enforce that only one instance of TextureHolder exists.
TextureHolder::TextureHolder() {
  // If msInstance is not null, another TextureHolder was already created.
  assert(msInstance == nullptr);
  // Set this object as the singleton instance.
  msInstance = &s_textureHolder;
}

sf::Texture &TextureHolder::GetTexture(const std::string &filename) {
  // Shortcut to the map of textures on the singleton instance.
  auto &texturesMap = msInstance->mTextures;

  // Try to find the texture in the map (cache lookup).
  auto it = texturesMap.find(filename);
  if (it != texturesMap.end()) {
    // Found: return existing texture reference.
    return it->second;
  }

  // Not found: operator[] will insert a new entry with key 'filename'
  // and default-construct the sf::Texture value.
  // uses std::map::operator[]. If filename isn’t already a key in the map, this
  // call:
  // 1. Default-constructs a new sf::Texture.
  // 2. Inserts it into texturesMap under the key filename.
  // 3. Returns a reference to that newly inserted sf::Texture.
  auto &texture = texturesMap[filename];

  // Attempt to load the image file into the texture.
  if (!texture.loadFromFile(filename)) {
    // Loading failed: print an error but still return the (empty) texture.
    std::cerr << "Failed to load texture: " << filename << std::endl;
  }

  // Return the newly loaded (or empty) texture.
  return texture;
}
