# Z-Buffer (Depth Buffer)

The **Z-buffer** is a 2D array that stores the depth (Z-coordinate) of each pixel to determine which objects are visible and which are hidden behind others.

## How It Works

1. **Initialize:** Set all Z-buffer values to maximum depth (far plane)
2. **For each pixel being drawn:**
   - Calculate the Z-depth of the current fragment
   - Compare with existing Z-buffer value at that pixel
   - If closer (smaller Z), update both color buffer and Z-buffer
   - If farther (larger Z), discard the fragment

## Example Scenario

Imagine rendering a scene with three objects:

```
Scene from side view:
Near ←─────────────→ Far
     A    B    C
   (1.0)(5.0)(8.0)    ← Z-coordinates

From camera view (top-down):
┌─────────────────┐
│     A     B     │  ← A and B overlap at pixel (100,100)
│       C         │  ← C is at pixel (150,150)  
└─────────────────┘
```

**Z-buffer process at pixel (100,100):**

1. **Initially:** Z-buffer[100][100] = ∞ (far plane)
2. **Draw object C (Z=8.0):**
   - 8.0 < ∞ → Update: Z-buffer[100][100] = 8.0, draw C
3. **Draw object B (Z=5.0):**  
   - 5.0 < 8.0 → Update: Z-buffer[100][100] = 5.0, draw B
4. **Draw object A (Z=1.0):**
   - 1.0 < 5.0 → Update: Z-buffer[100][100] = 1.0, draw A

**Result:** Only object A is visible at that pixel.

## Code Example

```cpp
// Pseudo-code for Z-buffer test
void drawPixel(int x, int y, float z, Color color) {
    if (z < zBuffer[x][y]) {        // Closer than current
        zBuffer[x][y] = z;          // Update depth
        colorBuffer[x][y] = color;  // Update color
    }
    // Else: fragment is behind, discard it
}
```

## OpenGL Z-Buffer

In OpenGL (which your code uses), the Z-buffer is automatically managed:

```cpp
// Enable depth testing
glEnable(GL_DEPTH_TEST);

// Clear both color and depth buffers each frame
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Set depth function (default is GL_LESS)
glDepthFunc(GL_LESS);  // Pass if incoming < stored
```

## Without Z-Buffer

Without Z-buffering, objects would be drawn in the order they appear in your code:

```cpp
// Wrong result - depends on draw order
draw(objectFar);    // Drawn first
draw(objectNear);   // Drawn second, appears in front (correct)

vs.

draw(objectNear);   // Drawn first  
draw(objectFar);    // Drawn second, incorrectly appears in front!
```

## Z-Fighting

When two surfaces have nearly identical Z values, they may flicker as they compete for visibility:

```cpp
// Both objects at almost same depth
objectA.z = 5.0000001f;
objectB.z = 5.0000002f;  // Very close - may cause z-fighting
```

**Solutions:**
- Increase separation between objects
- Use higher precision depth buffer (24-bit or 32-bit)
- Adjust near/far plane ratio

## Memory Usage

For a 1920×1080 screen:
- Z-buffer size: 1920 × 1080 × 4 bytes = ~8MB
- Each pixel stores one 32-bit float depth value

## Depth Testing Functions

OpenGL supports various depth test functions:

| Function | Test Passes When |
|----------|------------------|
| GL_NEVER | Never passes |
| GL_LESS | incoming < stored (default) |
| GL_EQUAL | incoming == stored |
| GL_LEQUAL | incoming <= stored |
| GL_GREATER | incoming > stored |
| GL_NOTEQUAL | incoming != stored |
| GL_GEQUAL | incoming >= stored |
| GL_ALWAYS | Always passes |

The Z-buffer is essential for correct 3D rendering - without it, objects would appear in the wrong order based purely on draw sequence rather than their actual 3D positions.