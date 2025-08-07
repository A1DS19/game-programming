# Matrices and Matrix Transformations in Game Programming

**Matrix:** A rectangular array of numbers arranged in rows and columns, used to represent and perform transformations in 3D space.

## What are Matrices?

A **4x4 matrix** is the standard in 3D graphics:
```
[m00  m01  m02  m03]
[m10  m11  m12  m13]
[m20  m21  m22  m23]
[m30  m31  m32  m33]
```

## Why Matrices?

1. **Combine Multiple Transformations**: One matrix can represent rotation, scaling, and translation
2. **Efficient**: GPU hardware is optimized for matrix operations
3. **Composable**: Multiple transformations can be combined by multiplying matrices
4. **Uniform**: All transformations use the same mathematical framework

## Types of Transformation Matrices

### 1. Identity Matrix
**No transformation (original position)**
```
[1  0  0  0]
[0  1  0  0]
[0  0  1  0]
[0  0  0  1]
```

### 2. Translation Matrix
**Move object to new position**
```
[1  0  0  tx]
[0  1  0  ty]
[0  0  1  tz]
[0  0  0  1 ]
```
Where `tx`, `ty`, `tz` are the translation amounts.

### 3. Scale Matrix
**Resize object**
```
[sx  0   0  0]
[0  sy   0  0]
[0   0  sz  0]
[0   0   0  1]
```
Where `sx`, `sy`, `sz` are scale factors.

### 4. Rotation Matrix (Z-axis)
**Rotate around Z-axis by angle θ**
```
[cos(θ) -sin(θ)  0  0]
[sin(θ)  cos(θ)  0  0]
[  0       0     1  0]
[  0       0     0  1]
```

## Matrix-Vector Multiplication

To transform a point (x, y, z) using a matrix:
```
[m00  m01  m02  m03]   [x]   [x']
[m10  m11  m12  m13] × [y] = [y']
[m20  m21  m22  m23]   [z]   [z']
[m30  m31  m32  m33]   [1]   [w']
```

**Example:** Translate point (2, 3, 1) by (5, -2, 0)
```
Translation matrix:
[1  0  0  5]
[0  1  0 -2]
[0  0  1  0]
[0  0  0  1]

[1  0  0  5]   [2]   [1×2 + 0×3 + 0×1 + 5×1]   [7]
[0  1  0 -2] × [3] = [0×2 + 1×3 + 0×1 + -2×1] = [1]
[0  0  1  0]   [1]   [0×2 + 0×3 + 1×1 + 0×1]   [1]
[0  0  0  1]   [1]   [0×2 + 0×3 + 0×1 + 1×1]   [1]

Result: (7, 1, 1)
```

## Combining Transformations

**Matrix multiplication combines transformations:**
```cpp
Matrix4 final = translationMatrix * rotationMatrix * scaleMatrix;
```

**Order matters!** Transformations are applied right to left:
1. Scale first
2. Then rotate
3. Then translate

### Example: Scale, Rotate, Translate
```cpp
// Scale by 2x
Matrix4 scale = Matrix4::CreateScale(2.0f);

// Rotate 90° around Z-axis  
Matrix4 rotation = Matrix4::CreateRotationZ(Math::PiOver2);

// Translate by (10, 5, 0)
Matrix4 translation = Matrix4::CreateTranslation(Vector3(10, 5, 0));

// Combine (order: translate * rotate * scale)
Matrix4 transform = translation * rotation * scale;
```

## Common Matrices in Game Engines

### Model Matrix
**Object's position, rotation, and scale in world space**
```cpp
Matrix4 modelMatrix = translation * rotation * scale;
```

### View Matrix  
**Camera's position and orientation**
```cpp
Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPos, target, up);
```

### Projection Matrix
**3D to 2D projection (perspective or orthographic)**
```cpp
Matrix4 projMatrix = Matrix4::CreatePerspectiveFOV(fov, aspect, near, far);
```

### Model-View-Projection (MVP) Matrix
**Complete transformation from object space to screen space**
```cpp
Matrix4 mvp = projMatrix * viewMatrix * modelMatrix;
```

## Practical Examples

### Rotating a Cube Around Y-axis
```cpp
float angle = time * rotationSpeed;
Matrix4 rotation = Matrix4::CreateRotationY(angle);
Matrix4 translation = Matrix4::CreateTranslation(cubePosition);
Matrix4 modelMatrix = translation * rotation;
```

### Orbiting Camera
```cpp
float angle = time * orbitSpeed;
Vector3 cameraPos(
    radius * cos(angle),  // X
    height,               // Y  
    radius * sin(angle)   // Z
);
Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPos, target, Vector3::UnitY);
```

### Scaling Based on Distance
```cpp
float distance = (cameraPos - objectPos).Length();
float scale = maxScale / (1.0f + distance * 0.1f);
Matrix4 scaleMatrix = Matrix4::CreateScale(scale);
```

## Why 4x4 Matrices?

**Homogeneous Coordinates:** The extra row/column allows:
1. **Translation**: Can be represented as matrix multiplication
2. **Perspective Division**: For 3D perspective projection
3. **Uniform Operations**: All transformations use same matrix size

**Point vs Vector:**
- Point (x, y, z, 1): Affected by translation
- Vector (x, y, z, 0): Not affected by translation (direction only)

## Matrix Benefits in Shaders

**Vertex Shader Example:**
```glsl
attribute vec3 position;
uniform mat4 mvpMatrix;

void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
}
```

Single matrix multiplication transforms vertex from object space to screen space!

## Summary

Matrices are the mathematical foundation for all 3D transformations in games. They provide:
- **Efficiency**: Hardware-optimized operations
- **Flexibility**: Combine any transformations
- **Uniformity**: Consistent mathematical framework
- **Composability**: Complex transforms from simple building blocks

Understanding matrices is essential for 3D graphics programming, camera systems, animation, and physics.
