#include "sound.hpp"
#include <iostream>

Sound::Sound(const std::string &filePath) : soundBuffer(), sound(soundBuffer) {
  if (!soundBuffer.loadFromFile(filePath)) {
    std::cerr << "sound file " << filePath << "not loaded" << std::endl;
  }

  std::cout << "sound file " << filePath << " loaded" << std::endl;

  sound.setBuffer(soundBuffer);
}
