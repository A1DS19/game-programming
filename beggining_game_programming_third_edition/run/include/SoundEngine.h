#pragma once

#include "SFML/Audio.hpp"

// Singleton pattern

class SoundEngine {
private:
  static sf::Music music;
  static sf::SoundBuffer mClickBuffer;
  static sf::Sound mClickSound;
  static sf::SoundBuffer mJumpBuffer;
  static sf::Sound mJumpSound;

public:
  SoundEngine();
  static SoundEngine *m_s_Instance;
  static bool mMusicIsPlaying;
  static void StartMusic();
  static void PauseMusic();
  static void ResumeMusic();
  static void StopMusic();
  static void PlayClick();
  static void PlayJump();
};
