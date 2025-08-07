# Polygon Transformations with Trigonometry

## Why Sin, Cos, and Tan?

Polygons use **sin**, **cos**, and **tan** for transformations because these trigonometric functions describe **circular motion** and **rotational relationships**.

### Basic Trigonometry
- **cos(θ)** = adjacent/hypotenuse = x-component of rotation
- **sin(θ)** = opposite/hypotenuse = y-component of rotation  
- **tan(θ)** = opposite/adjacent = slope of the line at angle θ

Together, cos and sin describe a point on a unit circle at angle θ.

## 2D Rotation

### Formula
To rotate point (x, y) by angle θ:
```
new_x = x * cos(θ) - y * sin(θ)
new_y = x * sin(θ) + y * cos(θ)
```

### Why This Works
- Any rotation can be decomposed into x and y components
- **cos(θ)** tells you how much of the original x-axis aligns with the new x-axis
- **sin(θ)** tells you how much of the original y-axis contributes to the new x-axis

### Matrix Form
```
[cos(θ) -sin(θ)]   [x]   [new_x]
[sin(θ)  cos(θ)] × [y] = [new_y]
```

## Examples

### Example 1: Rotate (1, 0) by 90°
```
cos(90°) = 0, sin(90°) = 1
new_x = 1 × 0 - 0 × 1 = 0
new_y = 1 × 1 + 0 × 0 = 1
Result: (0, 1) ✓
```

### Example 2: Rotate (2, 3) by 45°
```
cos(45°) = 0.707, sin(45°) = 0.707
new_x = 2 × 0.707 - 3 × 0.707 = 1.414 - 2.121 = -0.707
new_y = 2 × 0.707 + 3 × 0.707 = 1.414 + 2.121 = 3.535
Result: (-0.707, 3.535)
```

### Example 3: Rotate (4, 0) by 180°
```
cos(180°) = -1, sin(180°) = 0
new_x = 4 × (-1) - 0 × 0 = -4
new_y = 4 × 0 + 0 × (-1) = 0
Result: (-4, 0) ✓
```

### Example 4: Rotating a Complete Polygon 90°

**Given polygon with vertices:**
- A = (-2, -1)
- B = (0, 1) 
- C = (2, -1)

**Step 1: Set Up the Rotation Formula**

For any rotation by angle θ:
```
new_x = x × cos(θ) - y × sin(θ)
new_y = x × sin(θ) + y × cos(θ)
```

For 90° rotation:
- cos(90°) = 0
- sin(90°) = 1

So the formula becomes:
```
new_x = x × 0 - y × 1 = -y
new_y = x × 1 + y × 0 = x
```

**Step 2: Apply to Vertex A = (-2, -1)**

Original coordinates: x = -2, y = -1
```
new_x = -y = -(-1) = 1
new_y = x = -2
```
Result: A' = (1, -2)

**Step 3: Apply to Vertex B = (0, 1)**

Original coordinates: x = 0, y = 1
```
new_x = -y = -(1) = -1
new_y = x = 0
```
Result: B' = (-1, 0)

**Step 4: Apply to Vertex C = (2, -1)**

Original coordinates: x = 2, y = -1
```
new_x = -y = -(-1) = 1
new_y = x = 2
```
Result: C' = (1, 2)

**Final Result:**
- Original polygon: A(-2, -1), B(0, 1), C(2, -1)
- Rotated 90° counterclockwise: A'(1, -2), B'(-1, 0), C'(1, 2)

The polygon rotated 90° counterclockwise around the origin (0, 0).

## 3D Rotations

For 3D rotations, we need separate matrices for each axis:

### Rotation around X-axis
```
[1    0      0   ]   [x]
[0  cos(θ) -sin(θ)] × [y]
[0  sin(θ)  cos(θ)]   [z]
```

### Rotation around Y-axis
```
[ cos(θ)  0  sin(θ)]   [x]
[   0     1    0   ] × [y]
[-sin(θ)  0  cos(θ)]   [z]
```

### Rotation around Z-axis
```
[cos(θ) -sin(θ)  0]   [x]
[sin(θ)  cos(θ)  0] × [y]
[  0       0     1]   [z]
```

## Other Transformations

### Translation
```
new_x = x + tx
new_y = y + ty
```
No trigonometry needed - simple addition.

### Scaling
```
new_x = x * sx
new_y = y * sy
```
No trigonometry needed - simple multiplication.

### Shearing (uses tan)
```
new_x = x + tan(θ) * y
new_y = y
```

## Why Trigonometry is Essential

1. **Unit Circle**: cos and sin naturally describe points on a circle
2. **Periodic Nature**: Rotations are cyclic (360° = 0°)
3. **Orthogonal Components**: cos and sin are perpendicular, perfect for 2D coordinates
4. **Smooth Transitions**: Trigonometric functions provide smooth, continuous transformations

Trigonometry is the mathematical foundation for describing rotational motion in 2D/3D space.
