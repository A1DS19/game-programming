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

### 4. Render Loop - Detailed Step-by-Step (`GenerateOutput()`)

#### From Game Loop to Rendered Object:

**Game Loop** (`RunLoop()` → `GenerateOutput()`)
1. **Clear Screen** (`Game.cpp:198-199`)
   ```cpp
   glClearColor(0.86f, 0.86f, 0.86f, 0.86f);  // Set background color
   glClear(GL_COLOR_BUFFER_BIT);               // Clear back buffer
   ```

2. **Activate Shader Program** (`Game.cpp:201`)
   ```cpp
   mSpriteShader->SetActive();  // Calls glUseProgram()
   ```
   - **Shader class** (`Shader.cpp:19`) tells GPU which vertex/fragment shaders to use

3. **Bind Vertex Array** (`Game.cpp:202`)
   ```cpp
   mSpriteVerts->SetActive();   // Calls glBindVertexArray()
   ```
   - **VertexArray class** (`VertexArray.cpp:39`) binds vertex buffer and attributes
   - This contains our quad vertices (4 vertices forming 2 triangles)

4. **For Each Sprite** (`Game.cpp:205-207`)
   ```cpp
   for (auto sprite : mSprites) {
       sprite->Draw(mSpriteShader);  // SpriteComponent::Draw()
   }
   ```

5. **Sprite Draw Call** (`SpriteComponent.cpp:19-20`)
   ```cpp
   void SpriteComponent::Draw(Shader *shader) {
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
   }
   ```
   - **SpriteComponent** belongs to an **Actor** (`Actor.cpp`)
   - Issues the actual draw command to GPU
   - Draws 6 indices (2 triangles × 3 vertices each) as `GL_TRIANGLES`

6. **Swap Buffers** (`Game.cpp:209`)
   ```cpp
   SDL_GL_SwapWindow(mWindow);  // Display completed frame
   ```

#### Class Relationships:
- **Game** manages the main rendering loop
- **Actor** represents game objects with position, rotation, scale
- **SpriteComponent** handles rendering for actors (added to `mSprites` list)
- **VertexArray** manages vertex buffers on GPU (`VertexArray.cpp:4-31`)
- **Shader** compiles and manages vertex/fragment shader programs

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

**NDC (Normalized Device Coordinates)**:
- Standardized coordinate system used by OpenGL after transformations
- Range: X, Y, Z all go from -1.0 to +1.0
- (-1, -1) = bottom-left corner, (1, 1) = top-right corner
- Hardware-independent, works on any screen size

**Coordinate Transformation Flow**:
1. **World Coordinates** → Model Matrix
2. **View Coordinates** → View Matrix  
3. **Clip Coordinates** → Projection Matrix
4. **NDC** → Perspective divide (clip coords ÷ w component)
5. **Screen Coordinates** → Viewport transform

The vertex shader outputs clip coordinates (`gl_Position`), which OpenGL automatically converts to NDC.
