# OpenGL Rendering Pipeline

This document explains the steps from vertices to rendered polygons in our OpenGL game.

## What is an OpenGL Program?
An **OpenGL Program** is a compiled combination of vertex and fragment shaders that processes geometry on the GPU.

## Rendering Pipeline Steps

### 1. Define Vertices (`Game.cpp:87-101`)
```cpp
float vertices[] = {
    -0.5f, 0.5f, 0.f, 0.f, 0.f,  // position (x,y,z) + texture coords (u,v)
    0.5f,  0.5f, 0.f, 1.f, 0.f,  // top right
    0.5f, -0.5f, 0.f, 1.f, 1.f,  // bottom right
    -0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
};

unsigned int indices[] = {
    0, 1, 2,  // First triangle
    2, 3, 0   // Second triangle
};
```
- Creates a quad (4 vertices) that forms 2 triangles
- Each vertex has position (x,y,z) and texture coordinates (u,v)

### 2. Create Vertex Buffer (`VertexArray` constructor)
- Uploads vertex data to GPU memory (Vertex Buffer Object - VBO)
- Creates index buffer for efficient vertex reuse (Element Buffer Object - EBO)
- Sets up vertex attributes (tells GPU how to interpret the data)

### 3. Load & Compile Shaders (`LoadShaders()`)
- **Vertex Shader**: `../shaders/Basic.vert` - transforms vertex positions
- **Fragment Shader**: `../shaders/Basic.frag` - determines pixel colors
- Compiles both shaders and links them into a shader program

### 4. Render Loop (`GenerateOutput()`)
```cpp
mSpriteShader->SetActive();  // Tell GPU to use our shader program
mSpriteVerts->SetActive();   // Bind our vertex buffer
sprite->Draw(mSpriteShader); // Issue draw call
SDL_GL_SwapWindow(mWindow);  // Swap buffers to display frame
```

### 5. GPU Processing Pipeline
1. **Vertex Shader** - Processes each vertex individually
   - Transforms 3D positions to screen coordinates
   - Passes data to next stage

2. **Rasterization** - GPU converts triangles to pixels
   - Determines which pixels are inside each triangle
   - Interpolates vertex data across the triangle surface

3. **Fragment Shader** - Processes each pixel
   - Determines final color for each pixel
   - Applies textures, lighting, effects

4. **Buffer Swap** - Display the completed frame
   - Prevents screen tearing by showing complete frames only

## Key Concepts

**Polygons = Triangles**: All complex shapes are broken down into triangles because:
- Triangles are always planar (flat)
- Guaranteed to be convex
- Efficient for GPU processing

**Double Buffering**: 
- Front buffer = what's displayed
- Back buffer = what's being drawn
- Swap when frame is complete for smooth animation

**Vertex Buffer vs Index Buffer**:
- Vertex buffer stores actual vertex data
- Index buffer stores indices pointing to vertices
- Eliminates duplicate vertices, saves memory
