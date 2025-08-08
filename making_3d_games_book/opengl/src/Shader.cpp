#include "Shader.hpp"
#include "SDL_log.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader() : mVertexShader(0), mFragShader(0), mShaderProgram(0) {}

Shader::~Shader() {}

void Shader::SetMatrixUniform(const char *name, const Matrix4 &matrix) {
  GLuint loc = glGetUniformLocation(mShaderProgram, name);
  // Send the matrix data to the uniform
  glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::Unload() {
  // Delete the programs/shaders.
  glDeleteProgram(mShaderProgram);
  glDeleteShader(mVertexShader);
  glDeleteShader(mFragShader);
}

void Shader::SetActive() { glUseProgram(mShaderProgram); }

bool Shader::IsValidProgram() {
  GLint status;

  glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    memset(buffer, 0, 512);
    glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
    SDL_Log("GLSL link status: \n%s", buffer);
    return false;
  }

  return true;
}

bool Shader::Load(const std::string &vertName, const std::string &fragName) {
  // Compile vertex and fragment shaders.
  if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
      !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)) {
    return false;
  }

  // Create shader program.
  // link vertex/frag shaders.
  mShaderProgram = glCreateProgram();
  glAttachShader(mShaderProgram, mVertexShader);
  glAttachShader(mShaderProgram, mFragShader);
  glLinkProgram(mShaderProgram);

  // Verify that the program linked successfully.
  if (!IsValidProgram()) {
    return false;
  }

  return true;
}

bool Shader::IsCompiled(GLuint shader) {
  GLint status;
  // Query the compiled status.
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    char buffer[512];
    memset(buffer, 0, 512);
    glGetShaderInfoLog(shader, 511, nullptr, buffer);
    SDL_Log("GLSL compile failed:\n%s", buffer);
    return false;
  }

  return true;
}

bool Shader::CompileShader(const std::string &fileName, GLenum shaderType,
                           GLuint &outShader) {
  std::ifstream shaderFile(fileName);
  if (shaderFile.is_open()) {
    // Read all text into a string.
    std::stringstream sstream;
    sstream << shaderFile.rdbuf();

    std::string contents = sstream.str();
    const char *contentsChar = contents.c_str();

    // Create shader of a specified type.
    outShader = glCreateShader(shaderType);
    // Set the source characters and try to compile.
    glShaderSource(outShader, 1, &(contentsChar), nullptr);
    glCompileShader(outShader);

    if (!IsCompiled(outShader)) {
      SDL_Log("Failed to compile shader %s", fileName.c_str());
      return false;
    }
  } else {
    SDL_Log("Shader file not found: %s", fileName.c_str());
    return false;
  }

  return true;
}
