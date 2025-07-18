#pragma once

#include "SFML/Graphics.hpp"
#include <map>
#include <string>

class TextureHolder {
private:
  std::map<std::string, sf::Texture> mTextures;
  // The one instance to my texture holder.
  static TextureHolder *msInstance;

public:
  TextureHolder();
  static sf::Texture &GetTexture(const std::string &filename);
};
