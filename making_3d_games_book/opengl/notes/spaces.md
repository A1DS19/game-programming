# OpenGL Coordinate Spaces

This document outlines the different coordinate spaces used in the OpenGL rendering pipeline, from the initial object definition to the final image on the screen.

## The Transformation Pipeline

The journey of a vertex from its initial definition to the screen involves several transformations, each moving it into a new coordinate space. The primary spaces are:

1.  **Model Space**
2.  **World Space**
3.  **View Space (Camera Space)**
4.  **Clip Space**
5.  **Screen Space**

---

### 1. Model Space

*   **Concept:** This is the local coordinate system of an object. When you create a 3D model, its vertices are defined relative to its own origin (0,0,0). This makes the model independent of any specific scene.
*   **Example:** A car model would have its origin at the center of the model, and all its vertices would be defined relative to that point.

---

### 2. World Space

*   **Concept:** This is the global coordinate system for the entire 3D scene. All objects, lights, and the camera are placed within this single, unified space.
*   **Transformation:** The **model matrix** is used to transform an object's vertices from model space into world space. This matrix applies translation, rotation, and scaling to position the object correctly in the scene.

---

### 3. View Space (Camera Space)

*   **Concept:** This is the scene as seen from the camera's perspective. The camera is at the origin, and all other objects are positioned relative to it.
*   **Transformation:** The **view matrix** transforms the scene from world space into view space. This is equivalent to positioning and orienting the camera within the world.

---

### 4. Clip Space

*   **Concept:** This is a coordinate system where the visibility of vertices is determined. Any geometry that falls outside of a specific volume, known as the "clip volume" or "view frustum," is "clipped" or discarded. This is a highly efficient way to avoid rendering objects that are off-screen.
*   **Transformation:** The **projection matrix** transforms the scene from view space into clip space. It defines the shape of the viewing volume (the "frustum") and projects the 3D scene onto a 2D plane. The result is what you assign to `gl_Position` in the vertex shader.

---

### 5. Screen Space

*   **Concept:** This is the final 2D coordinate system of your screen or window. The coordinates are typically in pixels.
*   **Transformation:** The GPU automatically transforms the clip space coordinates into screen space. This involves perspective division (to create the illusion of depth) and mapping the coordinates to the viewport.

## Summary Diagram

Here is a simple diagram illustrating the flow:

```
[Model Space] --Model Matrix--> [World Space] --View Matrix--> [View Space] --Projection Matrix--> [Clip Space] --GPU--> [Screen Space]
```

The combination of the model, view, and projection matrices is often called the **Model-View-Projection (MVP) matrix**, which can transform a vertex directly from model space to clip space in a single operation.
