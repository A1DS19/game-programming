# Projection Types

## Orthographic Projection

**Orthographic projection** creates a "flat" view where parallel lines remain parallel and objects don't appear smaller with distance.

**Key characteristics:**
- No perspective distortion
- Objects maintain the same size regardless of depth
- Parallel lines stay parallel
- Often used for technical drawings, 2D games, UI elements

**How it works:**
- Simply drops the Z coordinate
- Maps 3D coordinates (x, y, z) to 2D coordinates (x, y)
- Uses a rectangular viewing volume (box)

**Matrix form:**
```
[1  0  0  0]
[0  1  0  0]
[0  0  0  0]  <- Z component is discarded
[0  0  0  1]
```

**Use cases:**
- CAD software
- 2D games with 3D graphics
- UI overlays
- Technical illustrations

## Perspective Projection

**Perspective projection** mimics how human eyes see - objects appear smaller as they get farther away.

**Key characteristics:**
- Objects farther away appear smaller
- Parallel lines converge to vanishing points
- Creates depth illusion
- More realistic representation

**How it works:**
- Uses division by depth (Z coordinate)
- Projects onto a viewing plane at distance d
- Formula: `x' = (d * x) / z`, `y' = (d * y) / z`

**Matrix form (simplified):**
```
[1  0  0  0]
[0  1  0  0]
[0  0  1  0]
[0  0  1/d 0]  <- Creates perspective division
```

**Parameters:**
- **Field of View (FOV):** How wide the view angle is
- **Aspect Ratio:** Width/height of viewport  
- **Near/Far planes:** Clipping distances

## Visual Comparison

```
Orthographic:           Perspective:
     ┌─────┐               ╱─────╲
     │  □  │              ╱   □   ╲
     │  □  │             ╱    □    ╲
     │  □  │            ╱     ■     ╲
     └─────┘           ╱_____________╲
   Same size boxes       Smaller with distance
```

## In Code Implementation

In `Math.hpp` (lines 598-614), both projections are available:

```cpp
// Orthographic projection
static Matrix4 CreateOrtho(float width, float height, float near, float far);

// Perspective projection  
static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height,
                                   float near, float far);
```

**When to use which:**
- **Orthographic:** 2D games, UI elements, technical views
- **Perspective:** 3D games, realistic scenes, first-person views

## Mathematical Details

### Orthographic Projection Matrix
```
[2/width    0         0              0        ]
[0          2/height  0              0        ]
[0          0         1/(far-near)   0        ]
[0          0         near/(near-far) 1       ]
```

### Perspective Projection Matrix  
```
[xScale  0       0                    0]
[0       yScale  0                    0]
[0       0       far/(far-near)       1]
[0       0       -near*far/(far-near) 0]
```

Where:
- `xScale = cot(fovY/2) * height/width`
- `yScale = cot(fovY/2)`