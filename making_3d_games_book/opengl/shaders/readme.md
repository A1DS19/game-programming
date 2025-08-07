# OpenGL Shaders

## Vertex Shader
**Processes each vertex individually.**

**What it does:**
- Transforms vertex positions (world → screen coordinates)
- Calculates lighting per vertex
- Passes data to fragment shader
- Runs once per vertex

**Example tasks:**
- Apply model/view/projection matrices
- Calculate vertex normals for lighting
- Transform texture coordinates

## Fragment Shader
**Processes each pixel (fragment) on the screen.**

**What it does:**
- Determines final pixel color
- Applies textures
- Calculates per-pixel lighting
- Runs once per pixel/fragment

**Example tasks:**
- Sample textures
- Calculate shadows
- Apply lighting effects
- Determine transparency

## Pipeline Flow
1. **Vertex Shader** → transforms triangle vertices
2. **Rasterization** → GPU creates fragments between vertices
3. **Fragment Shader** → colors each pixel inside the triangle

## Performance
- **Vertex shader:** Fewer calls (one per vertex)
- **Fragment shader:** Many more calls (one per pixel)

**Example:** A triangle with 3 vertices = 3 vertex shader calls, but might generate thousands of fragment shader calls (one per pixel the triangle covers).

## Core Graphics Pipeline
1. **Vertices** → **Vertex Shader**
   - Transforms 3D positions to screen coordinates
2. **GPU Rasterizer**
   - Converts triangles into pixels/fragments
   - Determines which pixels are inside each triangle
3. **Fragments** → **Fragment Shader**
   - Assigns final color to each pixel

**Simple Example:**
- Triangle has vertices at (0,0), (100,0), (50,100)
- Vertex shader transforms these positions
- Rasterizer finds all pixels inside the triangle (maybe 2,500 pixels)
- Fragment shader runs 2,500 times to color each pixel

The rasterizer is the bridge between geometry (vertices/triangles) and pixels (what you see on screen).
