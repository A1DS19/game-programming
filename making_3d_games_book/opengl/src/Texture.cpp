#include "SDL_log.h"
#define STB_IMAGE_IMPLEMENTATION

#include "GL/glew.h"
#include "SDL.h"
#include "Texture.hpp"
#include "stb_image.h"

Texture::Texture() : mTextureId(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

bool Texture::Load(const std::string &fileName) {
  int channels = 0;
  unsigned char *data =
      stbi_load(fileName.c_str(), &mWidth, &mHeight, &channels, 4);

  if (data) {
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    // Since we forced 4 channels (RGBA), always use GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    // Enable bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_Log("Successfully loaded texture %s (%dx%d)", fileName.c_str(), mWidth,
            mHeight);
    return true;
  } else {
    SDL_Log("Failed to load image %s: %s", fileName.c_str(),
            stbi_failure_reason());
    return false;
  }
}

void Texture::Unload() { glDeleteTextures(1, &mTextureId); }

void Texture::SetActive() { glBindTexture(GL_TEXTURE_2D, mTextureId); }
