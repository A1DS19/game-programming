# Vectors in Game Programming

**Vector:** A mathematical object representing both **magnitude** (length) and **direction**.

## Core Concepts

**2D Vector:** `Vector2(x, y)`
- **x**: horizontal component
- **y**: vertical component

**3D Vector:** `Vector3(x, y, z)`
- **x**: horizontal component
- **y**: vertical component  
- **z**: depth component

## Common Uses in Games

### 1. Position
```cpp
Vector3 playerPosition(10.5f, 0.0f, -5.2f);
```

### 2. Velocity/Movement
```cpp
Vector3 velocity(0.0f, 0.0f, 5.0f);  // Moving forward at 5 units/second
playerPosition += velocity * deltaTime;
```

### 3. Direction
```cpp
Vector3 forward(0.0f, 0.0f, 1.0f);   // Looking forward
Vector3 right(1.0f, 0.0f, 0.0f);     // Right direction
Vector3 up(0.0f, 1.0f, 0.0f);        // Up direction
```

## Vector Operations

### Addition
**Combine movements**
```cpp
Vector3 totalMovement = jumpVector + runVector;
```

### Subtraction
**Find direction between points**
```cpp
Vector3 direction = target - player;  // Points from player to target
```

### Scalar Multiplication
**Scale magnitude**
```cpp
Vector3 fastMovement = velocity * 2.0f;  // Double the speed
```

### Dot Product
**Measure similarity/angle**
```cpp
float similarity = vectorA.Dot(vectorB);
// Returns 1 if same direction, -1 if opposite, 0 if perpendicular
```

### Cross Product
**Find perpendicular vector (3D only)**
```cpp
Vector3 perpendicular = vectorA.Cross(vectorB);
```

### Normalization
**Make length = 1 (unit vector)**
```cpp
Vector3 direction = (target - player).Normalize();
```

## Practical Examples

### Player Movement
```cpp
Vector3 input(0, 0, 0);
if (keyW) input.z += 1;  // Forward
if (keyS) input.z -= 1;  // Backward
if (keyA) input.x -= 1;  // Left
if (keyD) input.x += 1;  // Right

input.Normalize();  // Prevent diagonal movement being faster
player.position += input * speed * deltaTime;
```

### Enemy AI - Chase Player
```cpp
Vector3 direction = (player.position - enemy.position).Normalize();
enemy.position += direction * enemySpeed * deltaTime;
```

### Collision Detection
```cpp
Vector3 distance = objectA.position - objectB.position;
if (distance.Length() < collisionRadius) {
    // Objects are colliding
}
```

### Calculate Distance Between Objects
```cpp
float distance = (playerPosition - enemyPosition).Length();
if (distance < attackRange) {
    // Enemy can attack player
}
```

### Projectile Physics
```cpp
Vector3 gravity(0.0f, -9.8f, 0.0f);  // Gravity pulls down
Vector3 initialVelocity(10.0f, 15.0f, 0.0f);  // Launch velocity

// Each frame:
velocity += gravity * deltaTime;  // Apply gravity
projectile.position += velocity * deltaTime;  // Update position
```

## Why Vectors are Essential

1. **Position Tracking**: Every object needs x, y, z coordinates
2. **Movement**: Velocity vectors determine how objects move
3. **Physics**: Forces, acceleration, collision all use vectors  
4. **Graphics**: Vertices, normals, lighting calculations
5. **AI**: Pathfinding, steering behaviors, decision making
6. **Camera**: Position, rotation, field of view calculations

Vectors are the fundamental building blocks for representing positions, movements, directions, and physics calculations in games.