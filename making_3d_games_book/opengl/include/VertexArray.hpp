#pragma once

/*
 Vertex Array: The concept of defining polygon vertices in an ordered list.

  Vertex Buffer (VBO): GPU memory that stores the actual vertex data (positions,
  colors, normals, texture coordinates).

  Index Buffer (EBO/IBO): GPU memory that stores indices pointing to vertices in
 the vertex buffer.

  How They Work Together:

  Without Index Buffer:
  // Vertex buffer for a quad (2 triangles)
  float vertices[] = {
      // Triangle 1
      -0.5f, -0.5f,  // vertex 0
       0.5f, -0.5f,  // vertex 1
       0.5f,  0.5f,  // vertex 2
      // Triangle 2
      -0.5f, -0.5f,  // vertex 0 (duplicate!)
       0.5f,  0.5f,  // vertex 2 (duplicate!)
      -0.5f,  0.5f   // vertex 3
  };

  With Index Buffer:
  // Vertex buffer (no duplicates)
  float vertices[] = {
      -0.5f, -0.5f,  // vertex 0
       0.5f, -0.5f,  // vertex 1
       0.5f,  0.5f,  // vertex 2
      -0.5f,  0.5f   // vertex 3
  };

  // Index buffer (references vertices)
  unsigned int indices[] = {
      0, 1, 2,  // Triangle 1
      0, 2, 3   // Triangle 2
  };

  Benefits:
  - Eliminates duplicate vertices
  - Saves memory
  - More efficient rendering
 */

class VertexArray {
public:
  VertexArray(const float *verts, unsigned int numVerts,
              const unsigned int *indices, unsigned int numIndices);
  ~VertexArray();

  // Activate vertex array to draw
  void SetActive();

  unsigned int GetNumIndices() const noexcept { return mNumIndices; }
  unsigned int GetNumVerts() const noexcept { return mNumVerts; }

private:
  // How many vertices in the vertex buffer?
  unsigned int mNumVerts;
  // How many indices in the index buffer
  unsigned int mNumIndices;
  // OpenGL ID of the vertex buffer
  unsigned int mVertexBuffer;
  // OpenGL ID of the index buffer
  unsigned int mIndexBuffer;
  // OpenGL ID of the vertex array object
  unsigned int mVertexArray;
};
