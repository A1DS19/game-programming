// GLSL version 3.3.0
#version 330

in vec3 inPosition;

void main(){
  gl_Position = vec4(inPosition, 1.0);
}
