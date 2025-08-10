#include "VertexArray.hpp"
#include "GL/glew.h"

VertexArray::VertexArray(const float *verts, unsigned int numVerts,
                         const unsigned int *indices, unsigned int numIndices)
    : mNumVerts(numVerts), mNumIndices(numIndices) {
  // Create vertex array object (VAO) - stores all vertex attribute state
  glGenVertexArrays(1, &mVertexArray);
  glBindVertexArray(mVertexArray);

  // Create vertex buffer object (VBO) - stores vertex data on GPU
  glGenBuffers(1, &mVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
  // Upload vertex data to GPU (8 floats per vertex: position(3) + normal(3) + texcoords(2))
  glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts,
               GL_STATIC_DRAW);

  // Create index buffer object (EBO) - stores triangle indices for indexed drawing
  glGenBuffers(1, &mIndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
  // Upload index data to GPU
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int),
               indices, GL_STATIC_DRAW);

  // Configure vertex attribute pointers to tell OpenGL how to interpret vertex data
  // Vertex format: [x, y, z, nx, ny, nz, u, v] (8 floats total per vertex)
  
  // Position attribute (location 0): 3 floats starting at offset 0
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

  // Normal attribute (location 1): 3 floats starting at offset 12 bytes (3 floats)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(sizeof(float) * 3));

  // Texture coordinates attribute (location 2): 2 floats starting at offset 24 bytes (6 floats)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(sizeof(float) * 6));
}

VertexArray::~VertexArray() {
  // Clean up OpenGL resources
  glDeleteBuffers(1, &mVertexBuffer);
  glDeleteBuffers(1, &mIndexBuffer);
  glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() { 
  // Bind this VAO to make it the active vertex array for rendering
  glBindVertexArray(mVertexArray); 
}
