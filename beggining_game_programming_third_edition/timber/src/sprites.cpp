#include "sprites.hpp"
#include "iostream"

bool loadTexture(sf::Texture &texture, const std::string &fileName) {
  if (!texture.loadFromFile(fileName)) {
    std::cerr << "could not load texture" << std::endl;
    return false;
  }

  return true;
}
