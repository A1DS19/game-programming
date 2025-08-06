#pragma once

#include "GL/glew.h"
#include "Math.hpp"
#include <string>

class Shader {
public:
  Shader();
  ~Shader();
  // Load vertex/fragment shaders with given names.
  bool Load(const std::string &vertName, const std::string &fragName);
  void Unload();
  // Set this as the active shader program.
  void SetActive();
  // Sets the matrix as uniform.
  void SetMatrixUniform(const char *name, const Matrix4 &matrix);

private:
  // Tries to compile the specified shader/
  bool CompileShader(const std::string &fileName, GLenum shaderType,
                     GLuint &outShader);
  // Tests if shader is compiled.
  bool IsCompiled(GLuint shader);
  // Tests if vertex/fragment programs link
  bool IsValidProgram();

private:
  // Shader obejct Ids
  GLuint mVertexShader;
  GLuint mFragShader;
  GLuint mShaderProgram;
};
