# Sprite Shader Pipeline

## Sprite.vert (Vertex Shader)

The vertex shader transforms sprite vertices from object space to clip space for 2D sprite rendering.

### Inputs:
- `inPosition` (vec3): Vertex position in object space
- `inTexCoord` (vec2): Texture coordinates (UV mapping)

### Uniforms:
- `uWorldTransform` (mat4): Transforms from object space to world space
- `uViewProj` (mat4): Combined view-projection matrix for camera transformation

### Process:
1. Converts 3D position to homogeneous coordinates (adds w=1.0)
2. Applies world transformation to move sprite to world space
3. Applies view-projection to convert to clip space: `pos * uWorldTransform * uViewProj`
4. Passes texture coordinates to fragment shader

### Output:
- `gl_Position`: Final vertex position in clip space
- `fragTexCoord`: Interpolated texture coordinates for fragment shader

## Sprite.frag (Fragment Shader)

The fragment shader samples the sprite texture and outputs the final pixel color.

### Inputs:
- `fragTexCoord` (vec2): Interpolated texture coordinates from vertex shader

### Uniforms:
- `uTexture` (sampler2D): The sprite texture to sample

### Process:
1. Uses `texture()` function to sample color from the bound texture
2. Uses interpolated texture coordinates to determine which pixel to sample

### Output:
- `outColor` (vec4): Final RGBA color written to the color buffer

## Pipeline Flow:
1. Vertex shader positions sprite vertices in 3D space
2. GPU rasterizes triangles, interpolating texture coordinates across pixels
3. Fragment shader samples texture at each pixel location
4. Result is a textured sprite rendered in 3D space
