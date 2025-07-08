#pragma once

#include "SFML/Audio.hpp"
#include <string>

struct Sound {
  Sound(const std::string &filePath);

  sf::SoundBuffer soundBuffer;
  sf::Sound sound;
};
