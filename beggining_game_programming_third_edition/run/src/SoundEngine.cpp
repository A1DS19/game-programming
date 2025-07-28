#include "SoundEngine.h"
#include <cassert>
#include <iostream>

SoundEngine *SoundEngine::m_s_Instance = nullptr;
bool SoundEngine::mMusicIsPlaying = false;
sf::Music SoundEngine::music;

sf::SoundBuffer SoundEngine::mClickBuffer;
sf::Sound SoundEngine::mClickSound(SoundEngine::mClickBuffer);

sf::SoundBuffer SoundEngine::mJumpBuffer;
sf::Sound SoundEngine::mJumpSound(SoundEngine::mJumpBuffer);

SoundEngine::SoundEngine() {
  assert(m_s_Instance == nullptr);
  m_s_Instance = this;

  if (!mClickBuffer.loadFromFile("../assets/sound/click.wav")) {
    std::cout << "click sound not loaded" << std::endl;
    return;
  }
  mClickSound.setBuffer(mClickBuffer);

  if (!mJumpBuffer.loadFromFile("../assets/sound/jump.wav")) {
    std::cout << "jump sound not loaded" << std::endl;
    return;
  }
  mJumpSound.setBuffer(mJumpBuffer);
}

void SoundEngine::PlayClick() { mClickSound.play(); }

void SoundEngine::PlayJump() { mJumpSound.play(); }

void SoundEngine::StartMusic() {
  if (!music.openFromFile("../assets/music/music.wav")) {
    std::cout << "music not loaded" << std::endl;
    return;
  }

  m_s_Instance->music.play();
  m_s_Instance->music.setLooping(true);
  mMusicIsPlaying = true;
}

void SoundEngine::PauseMusic() {
  m_s_Instance->music.pause();
  mMusicIsPlaying = false;
}

void SoundEngine::ResumeMusic() {
  m_s_Instance->music.play();
  mMusicIsPlaying = true;
}

void SoundEngine::StopMusic() {
  m_s_Instance->music.stop();
  mMusicIsPlaying = false;
}
