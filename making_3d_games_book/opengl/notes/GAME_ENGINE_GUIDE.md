# Complete Step-by-Step 3D Game Engine Creation Guide

This guide documents the complete process of creating a 3D game engine using OpenGL, SDL, and C++. The engine uses an Entity-Component-System architecture with modern OpenGL rendering.

## Table of Contents
1. [Project Setup](#1-project-setup)
2. [Window Creation and OpenGL Context](#2-window-creation-and-opengl-context)
3. [Basic Renderer Setup](#3-basic-renderer-setup)
4. [Shader System](#4-shader-system)
5. [Math Library](#5-math-library)
6. [Actor and Component System](#6-actor-and-component-system)
7. [Mesh Loading and Rendering](#7-mesh-loading-and-rendering)
8. [Lighting System](#8-lighting-system)
9. [Camera and Movement](#9-camera-and-movement)
10. [Input Handling](#10-input-handling)
11. [Game Loop Integration](#11-game-loop-integration)
12. [Advanced Features](#12-advanced-features)

---

## 1. Project Setup

### Dependencies Required
- **SDL2**: Window management and input handling
- **GLEW**: OpenGL extension loading
- **CMake**: Build system

### Directory Structure
```
project/
├── CMakeLists.txt
├── include/           # Header files
├── src/              # Source files
├── shaders/          # GLSL shader files
├── assets/           # Textures, meshes, sounds
└── build/            # Build output
```

### CMakeLists.txt Setup
```cmake
cmake_minimum_required(VERSION 3.10)
project(GameEngine)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Add all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 GLEW::GLEW OpenGL::GL)
```

---

## 2. Window Creation and OpenGL Context

### Step 2.1: Create the Main Game Class

Create `include/Game.hpp`:
```cpp
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Actor;
class Renderer;
class CameraActor;

class Game {
public:
    static constexpr float SCREEN_WIDTH = 1024.0f;
    static constexpr float SCREEN_HEIGHT = 768.0f;

    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);
    class Renderer* GetRenderer() { return mRenderer; }

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    std::vector<Actor*> mActors;
    std::vector<Actor*> mPendingActors;
    
    class Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;
    
    CameraActor* mCameraActor;
};
```

### Step 2.2: Implement Game Initialization

Create `src/Game.cpp`:
```cpp
#include "Game.hpp"
#include "Renderer.hpp"
#include "SDL_log.h"
#include "SDL_timer.h"

Game::Game() : mRenderer{nullptr}, mIsRunning{true}, mUpdatingActors{false} {}

bool Game::Initialize() {
    // Initialize SDL with video and audio
    int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (sdlResult != 0) {
        SDL_Log("Failed to initialize with audio: %s", SDL_GetError());
        // Fall back to video only
        sdlResult = SDL_Init(SDL_INIT_VIDEO);
        if (sdlResult != 0) {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            return false;
        }
    }

    // Create renderer (handles OpenGL context creation)
    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        SDL_Log("Failed to initialize renderer");
        delete mRenderer;
        mRenderer = nullptr;
        return false;
    }

    LoadData();
    mTicksCount = SDL_GetTicks();
    return true;
}
```

### Step 2.3: Main Entry Point

Create `src/main.cpp`:
```cpp
#include "Game.hpp"

int main() {
    Game game;
    auto success = game.Initialize();

    if (success) {
        game.RunLoop();
    }

    game.Shutdown();
    return 0;
}
```

---

## 3. Basic Renderer Setup

### Step 3.1: Create Renderer Class

Create `include/Renderer.hpp`:
```cpp
#pragma once
#include "Math.hpp"
#include "SDL_video.h"
#include <string>
#include <unordered_map>
#include <vector>

struct DirectionalLight {
    Vector3 mDirection;
    Vector3 mDiffuseColor;
    Vector3 mSpecColor;
};

class Renderer {
public:
    Renderer(class Game* game);
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    void AddMeshComp(class MeshComponent* mesh);
    void RemoveMeshComp(class MeshComponent* mesh);

    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);

    void SetViewMatrix(const Matrix4& view) { mView = view; }
    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    DirectionalLight& GetDirectionalLight() { return mDirLight; }

    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }

private:
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader);

    // Resource management
    std::unordered_map<std::string, class Texture*> mTextures;
    std::unordered_map<std::string, class Mesh*> mMeshes;

    // Rendering lists
    std::vector<class SpriteComponent*> mSprites;
    std::vector<class MeshComponent*> mMeshComps;

    // Shaders and rendering data
    class Game* mGame;
    class Shader* mSpriteShader;
    class Shader* mMeshShader;
    class VertexArray* mSpriteVerts;

    // Matrices
    Matrix4 mView;
    Matrix4 mProjection;
    float mScreenWidth;
    float mScreenHeight;

    // Lighting
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;

    // SDL/OpenGL objects
    SDL_Window* mWindow;
    SDL_GLContext mContext;
};
```

### Step 3.2: Implement Renderer Initialization

Create `src/Renderer.cpp` (key sections):
```cpp
#include "Renderer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include <GL/glew.h>
#include <SDL_log.h>

bool Renderer::Initialize(float screenWidth, float screenHeight) {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Create window
    mWindow = SDL_CreateWindow("3D Game Engine", 100, 100,
                              static_cast<int>(mScreenWidth),
                              static_cast<int>(mScreenHeight),
                              SDL_WINDOW_OPENGL);
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create OpenGL context
    mContext = SDL_GL_CreateContext(mWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        SDL_Log("Failed to initialize GLEW.");
        return false;
    }

    // Clear any GLEW errors
    glGetError();

    if (!LoadShaders()) {
        SDL_Log("Failed to load shaders.");
        return false;
    }

    CreateSpriteVerts();

    return true;
}
```

---

## 4. Shader System

### Step 4.1: Create Shader Class

Create `include/Shader.hpp`:
```cpp
#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.hpp"

class Shader {
public:
    Shader();
    ~Shader();

    // Load vertex and fragment shaders
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload();
    
    // Set this as active shader
    void SetActive();

    // Set uniforms
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    void SetVectorUniform(const char* name, const Vector3& vector);
    void SetFloatUniform(const char* name, float value);

private:
    bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
    bool IsCompiled(GLuint shader);
    bool IsValidProgram();

    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
```

### Step 4.2: Implement Shader Loading

Create `src/Shader.cpp`:
```cpp
#include "Shader.hpp"
#include <SDL_log.h>
#include <fstream>
#include <sstream>

bool Shader::Load(const std::string& vertName, const std::string& fragName) {
    // Compile vertex and fragment shaders
    if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
        !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)) {
        return false;
    }

    // Create shader program
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    if (!IsValidProgram()) {
        return false;
    }

    return true;
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
    // Open file
    std::ifstream shaderFile(fileName);
    if (shaderFile.is_open()) {
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents = sstream.str();
        const char* contentsChar = contents.c_str();

        // Create shader
        outShader = glCreateShader(shaderType);
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if (!IsCompiled(outShader)) {
            SDL_Log("Failed to compile shader %s", fileName.c_str());
            return false;
        }
    } else {
        SDL_Log("Shader file not found: %s", fileName.c_str());
        return false;
    }

    return true;
}
```

### Step 4.3: Create Basic Shaders

Create `shaders/BasicMesh.vert`:
```glsl
#version 330

// Attributes
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Outputs
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    pos = pos * uWorldTransform;
    fragWorldPos = pos.xyz;
    gl_Position = pos * uViewProj;
    
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;
    fragTexCoord = inTexCoord;
}
```

Create `shaders/BasicMesh.frag`:
```glsl
#version 330

// Inputs
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

// Uniforms
uniform sampler2D uTexture;

// Output
out vec4 outColor;

void main() {
    outColor = texture(uTexture, fragTexCoord);
}
```

---

## 5. Math Library

### Step 5.1: Create Math Header

Create `include/Math.hpp`:
```cpp
#pragma once
#include <cmath>

namespace Math {
    const float Pi = 3.1415926535f;
    const float TwoPi = Pi * 2.0f;
    const float PiOver2 = Pi / 2.0f;
    const float Infinity = std::numeric_limits<float>::infinity();
    const float NegInfinity = -std::numeric_limits<float>::infinity();

    inline float ToRadians(float degrees) {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(float radians) {
        return radians * 180.0f / Pi;
    }
}

// 3D Vector
struct Vector3 {
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    explicit Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

    // Vector operations
    Vector3 operator+(const Vector3& a) const { return Vector3(x + a.x, y + a.y, z + a.z); }
    Vector3& operator+=(const Vector3& a) { x += a.x; y += a.y; z += a.z; return *this; }
    Vector3 operator-(const Vector3& a) const { return Vector3(x - a.x, y - a.y, z - a.z); }
    Vector3& operator-=(const Vector3& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }

    // Utility functions
    float LengthSq() const { return x*x + y*y + z*z; }
    float Length() const { return sqrtf(LengthSq()); }
    void Normalize() { float length = Length(); x /= length; y /= length; z /= length; }
    Vector3 GetNormalized() const { Vector3 temp = *this; temp.Normalize(); return temp; }

    // Dot product
    static float Dot(const Vector3& a, const Vector3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
    
    // Cross product
    static Vector3 Cross(const Vector3& a, const Vector3& b) {
        return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }

    // Common vectors
    static const Vector3 Zero;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
};

// Quaternion for rotations
struct Quaternion {
    float x, y, z, w;

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    explicit Quaternion(float inX, float inY, float inZ, float inW)
        : x(inX), y(inY), z(inZ), w(inW) {}
    explicit Quaternion(const Vector3& axis, float angle);

    void Conjugate() { x *= -1.0f; y *= -1.0f; z *= -1.0f; }
    float LengthSq() const { return x*x + y*y + z*z + w*w; }
    float Length() const { return sqrtf(LengthSq()); }
    void Normalize() { float length = Length(); x /= length; y /= length; z /= length; w /= length; }

    static Quaternion Concatenate(const Quaternion& q, const Quaternion& p);
    static const Quaternion Identity;
};

// 4x4 Matrix
struct Matrix4 {
    float mat[4][4];

    Matrix4();
    explicit Matrix4(float inMat[4][4]);

    // Matrix operations
    Matrix4 operator*(const Matrix4& a) const;
    Matrix4& operator*=(const Matrix4& a) { *this = *this * a; return *this; }

    // Transform a vector (assume w = 1.0f)
    Vector3 TransformPoint(const Vector3& vec) const;
    Vector3 TransformVector(const Vector3& vec) const;

    void Invert();
    Matrix4 GetInverse() const { Matrix4 temp = *this; temp.Invert(); return temp; }
    void Transpose();

    // Create specific transform matrices
    static Matrix4 CreateScale(float xScale, float yScale, float zScale);
    static Matrix4 CreateScale(const Vector3& scaleVector);
    static Matrix4 CreateScale(float scale);
    static Matrix4 CreateRotationX(float theta);
    static Matrix4 CreateRotationY(float theta);
    static Matrix4 CreateRotationZ(float theta);
    static Matrix4 CreateFromQuaternion(const Quaternion& q);
    static Matrix4 CreateTranslation(const Vector3& trans);
    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
    static Matrix4 CreateOrtho(float width, float height, float near, float far);
    static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far);

    static const Matrix4 Identity;
};
```

---

## 6. Actor and Component System

### Step 6.1: Create Base Actor Class

Create `include/Actor.hpp`:
```cpp
#pragma once
#include "Math.hpp"
#include <vector>
#include <cstdint>

class Actor {
public:
    enum State {
        EActive,
        EPaused,
        EDead
    };

    Actor(class Game* game);
    virtual ~Actor();

    // Update function called by game
    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime);

    // ProcessInput function called by game
    void ProcessInput(const uint8_t* keyState);
    virtual void ActorInput(const uint8_t* keyState);

    // Getters/setters
    const Vector3& GetPosition() const { return mPosition; }
    void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
    const Quaternion& GetRotation() const { return mRotation; }
    void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

    Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }

    class Game* GetGame() { return mGame; }
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }

    // Add/remove components
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

private:
    void ComputeWorldTransform();

    State mState;
    Vector3 mPosition;
    Quaternion mRotation;
    float mScale;
    Matrix4 mWorldTransform;
    bool mRecomputeWorldTransform;

    std::vector<class Component*> mComponents;
    class Game* mGame;
};
```

### Step 6.2: Implement Actor Class

Create `src/Actor.cpp`:
```cpp
#include "Actor.hpp"
#include "Game.hpp"
#include "Component.hpp"
#include <algorithm>

Actor::Actor(Game* game)
    : mState(EActive), mPosition(Vector3::Zero), mScale(1.0f),
      mRotation(Quaternion::Identity), mGame(game),
      mRecomputeWorldTransform(true) {
    mGame->AddActor(this);
}

Actor::~Actor() {
    mGame->RemoveActor(this);
    
    // Delete components
    while (!mComponents.empty()) {
        delete mComponents.back();
    }
}

void Actor::Update(float deltaTime) {
    if (mState == EActive) {
        ComputeWorldTransform();
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
        ComputeWorldTransform();
    }
}

void Actor::UpdateComponents(float deltaTime) {
    for (auto comp : mComponents) {
        comp->Update(deltaTime);
    }
}

void Actor::ProcessInput(const uint8_t* keyState) {
    if (mState == EActive) {
        for (auto comp : mComponents) {
            comp->ProcessInput(keyState);
        }
        ActorInput(keyState);
    }
}

void Actor::AddComponent(Component* component) {
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); ++iter) {
        if (myOrder < (*iter)->GetUpdateOrder()) {
            break;
        }
    }
    mComponents.insert(iter, component);
}

void Actor::ComputeWorldTransform() {
    if (mRecomputeWorldTransform) {
        mRecomputeWorldTransform = false;
        mWorldTransform = Matrix4::CreateScale(mScale);
        mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
        mWorldTransform *= Matrix4::CreateTranslation(mPosition);

        for (auto comp : mComponents) {
            comp->OnUpdateWorldTransform();
        }
    }
}
```

### Step 6.3: Create Base Component Class

Create `include/Component.hpp`:
```cpp
#pragma once
#include <cstdint>

class Component {
public:
    Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void Update(float deltaTime) {}
    virtual void ProcessInput(const uint8_t* keyState) {}
    virtual void OnUpdateWorldTransform() {}

    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    class Actor* mOwner;
    int mUpdateOrder;
};
```

---

## 7. Mesh Loading and Rendering

### Step 7.1: Create Vertex Array Class

Create `include/VertexArray.hpp`:
```cpp
#pragma once
#include <GL/glew.h>

class VertexArray {
public:
    VertexArray(const float* verts, unsigned int numVerts,
               const unsigned int* indices, unsigned int numIndices);
    ~VertexArray();

    // Activate this vertex array (set as active one to draw)
    void SetActive();

    unsigned int GetNumIndices() const { return mNumIndices; }
    unsigned int GetNumVerts() const { return mNumVerts; }

private:
    unsigned int mNumVerts;
    unsigned int mNumIndices;
    
    // OpenGL IDs
    unsigned int mVertexBuffer;
    unsigned int mIndexBuffer;
    unsigned int mVertexArray;
};
```

### Step 7.2: Create Mesh Class

Create `include/Mesh.hpp`:
```cpp
#pragma once
#include <vector>
#include <string>
#include "Math.hpp"

class Mesh {
public:
    Mesh();
    ~Mesh();

    // Load/unload mesh
    bool Load(const std::string& fileName, class Renderer* renderer);
    void Unload();

    // Get vertex array associated with this mesh
    class VertexArray* GetVertexArray() { return mVertexArray; }
    
    // Get texture from specified index
    class Texture* GetTexture(size_t index);
    
    // Get name of shader
    const std::string& GetShaderName() const { return mShaderName; }
    
    // Get object space bounding sphere radius
    float GetRadius() const { return mRadius; }
    
    // Get specular power of surface
    float GetSpecPower() const { return mSpecPower; }

private:
    // Textures associated with this mesh
    std::vector<class Texture*> mTextures;
    
    // Vertex array associated with this mesh
    class VertexArray* mVertexArray;
    
    // Name of shader specified by mesh
    std::string mShaderName;
    
    // Stores object space bounding sphere radius
    float mRadius;
    
    // Specular power of surface
    float mSpecPower;
};
```

### Step 7.3: Create Mesh Component

Create `include/MeshComponent.hpp`:
```cpp
#pragma once
#include "Component.hpp"
#include <cstddef>

class MeshComponent : public Component {
public:
    MeshComponent(class Actor* owner);
    ~MeshComponent();

    // Draw this mesh component
    virtual void Draw(class Shader* shader);
    
    // Set mesh/texture index used by mesh component
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
    void SetTextureIndex(size_t index) { mTextureIndex = index; }

protected:
    class Mesh* mMesh;
    size_t mTextureIndex;
};
```

### Step 7.4: Implement Mesh Component

Create `src/MeshComponent.cpp`:
```cpp
#include "MeshComponent.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Actor.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

MeshComponent::MeshComponent(Actor* owner)
    : Component(owner), mMesh(nullptr), mTextureIndex(0) {
    mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() {
    mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader) {
    if (mMesh) {
        // Set world transform
        shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
        
        // Set specular power
        shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
        
        // Set texture
        Texture* t = mMesh->GetTexture(mTextureIndex);
        if (t) {
            t->SetActive();
        }
        
        // Set vertex array
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        
        // Draw
        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}
```

---

## 8. Lighting System

### Step 8.1: Create Phong Shaders

Create `shaders/Phong.vert`:
```glsl
#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    pos = pos * uWorldTransform;
    fragWorldPos = pos.xyz;
    
    gl_Position = pos * uViewProj;
    
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;
    fragTexCoord = inTexCoord;
}
```

Create `shaders/Phong.frag`:
```glsl
#version 330

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outColor;

// Lighting uniforms
uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;

// Directional light
uniform vec3 uDirLight.mDirection;
uniform vec3 uDirLight.mDiffuseColor;
uniform vec3 uDirLight.mSpecColor;

uniform float uSpecPower;
uniform sampler2D uTexture;

void main() {
    // Surface normal
    vec3 N = normalize(fragNormal);
    
    // Vector from surface to light
    vec3 L = normalize(-uDirLight.mDirection);
    
    // Vector from surface to camera
    vec3 V = normalize(uCameraPos - fragWorldPos);
    
    // Reflection of -L about N
    vec3 R = normalize(reflect(-L, N));

    // Phong reflection
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if (NdotL > 0) {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
        Phong += Diffuse + Specular;
    }

    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
```

### Step 8.2: Implement Lighting in Renderer

Add to `src/Renderer.cpp`:
```cpp
void Renderer::SetLightUniforms(Shader* shader) {
    // Camera position is from inverted view
    Matrix4 invView = mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    
    // Ambient light
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    
    // Directional light
    shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}
```

---

## 9. Camera and Movement

### Step 9.1: Create Camera Actor

Create `include/CameraActor.hpp`:
```cpp
#pragma once
#include "Actor.hpp"

class CameraActor : public Actor {
public:
    CameraActor(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keys) override;

private:
    class MoveComponent* mMoveComp;
};
```

### Step 9.2: Implement Camera Actor

Create `src/CameraActor.cpp`:
```cpp
#include "CameraActor.hpp"
#include "MoveComponent.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "SDL_scancode.h"

CameraActor::CameraActor(Game* game) : Actor(game) {
    mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    // Compute new camera view matrix
    Vector3 cameraPos = GetPosition();
    Vector3 target = GetPosition() + GetForward() * 100.0f;
    Vector3 up = Vector3::UnitZ;

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keys) {
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;

    // WASD movement
    if (keys[SDL_SCANCODE_W]) {
        forwardSpeed += 300.0f;
    }
    if (keys[SDL_SCANCODE_S]) {
        forwardSpeed -= 300.0f;
    }
    if (keys[SDL_SCANCODE_A]) {
        angularSpeed -= Math::TwoPi;
    }
    if (keys[SDL_SCANCODE_D]) {
        angularSpeed += Math::TwoPi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);
}
```

### Step 9.3: Create Move Component

Create `include/MoveComponent.hpp`:
```cpp
#pragma once
#include "Component.hpp"

class MoveComponent : public Component {
public:
    MoveComponent(class Actor* owner, int updateOrder = 10);

    void Update(float deltaTime) override;

    float GetAngularSpeed() const { return mAngularSpeed; }
    float GetForwardSpeed() const { return mForwardSpeed; }
    void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
    float mAngularSpeed;
    float mForwardSpeed;
};
```

### Step 9.4: Implement Move Component

Create `src/MoveComponent.cpp`:
```cpp
#include "MoveComponent.hpp"
#include "Actor.hpp"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
    : Component(owner, updateOrder), mAngularSpeed(0.0f), mForwardSpeed(0.0f) {}

void MoveComponent::Update(float deltaTime) {
    if (!Math::NearZero(mAngularSpeed)) {
        Quaternion rot = mOwner->GetRotation();
        float angle = mAngularSpeed * deltaTime;
        Quaternion inc(Vector3::UnitZ, angle);
        rot = Quaternion::Concatenate(rot, inc);
        mOwner->SetRotation(rot);
    }

    if (!Math::NearZero(mForwardSpeed)) {
        Vector3 pos = mOwner->GetPosition();
        pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
        mOwner->SetPosition(pos);
    }
}
```

---

## 10. Input Handling

### Step 10.1: Implement Game Input Processing

Add to `src/Game.cpp`:
```cpp
void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // Process actor input
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->ProcessInput(keyboardState);
    }
    mUpdatingActors = false;
}
```

### Step 10.2: Create Input Component Base

Create `include/InputComponent.hpp`:
```cpp
#pragma once
#include "MoveComponent.hpp"

class InputComponent : public MoveComponent {
public:
    InputComponent(class Actor* owner);

    void ProcessInput(const uint8_t* keyState) override;

    // Getters/setters for keys
    int GetForwardKey() const { return mForwardKey; }
    int GetBackKey() const { return mBackKey; }
    int GetClockwiseKey() const { return mClockwiseKey; }
    int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }
    void SetForwardKey(int key) { mForwardKey = key; }
    void SetBackKey(int key) { mBackKey = key; }
    void SetClockwiseKey(int key) { mClockwiseKey = key; }
    void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }

    // Getters/setters for speed
    float GetMaxForwardSpeed() const { return mMaxForwardSpeed; }
    float GetMaxAngularSpeed() const { return mMaxAngularSpeed; }
    void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
    void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }

private:
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;
    
    int mForwardKey;
    int mBackKey;
    int mClockwiseKey;
    int mCounterClockwiseKey;
};
```

---

## 11. Game Loop Integration

### Step 11.1: Implement Complete Game Loop

Complete `src/Game.cpp`:
```cpp
void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::UpdateGame() {
    // Frame rate limiting (≈60 FPS)
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    // Calculate delta time
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05) {
        deltaTime = 0.05;
    }
    mTicksCount = SDL_GetTicks();

    // Update all actors
    mUpdatingActors = true;
    for (auto actor : mActors) {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // Move pending actors to active
    for (auto pending : mPendingActors) {
        pending->ComputeWorldTransform();
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // Clean up dead actors
    std::vector<Actor*> deadActors;
    for (auto actor : mActors) {
        if (actor->GetState() == Actor::EDead) {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors) {
        delete actor;
    }
}

void Game::GenerateOutput() {
    mRenderer->Draw();
}
```

### Step 11.2: Complete Renderer Draw Function

Add to `src/Renderer.cpp`:
```cpp
void Renderer::Draw() {
    // Clear color and depth buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw 3D meshes
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    mMeshShader->SetActive();
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
    SetLightUniforms(mMeshShader);
    
    for (auto mc : mMeshComps) {
        mc->Draw(mMeshShader);
    }

    // Draw 2D sprites (UI)
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();
    
    for (auto sprite : mSprites) {
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}
```

### Step 11.3: Load Initial Game Data

Add to `src/Game.cpp`:
```cpp
void Game::LoadData() {
    // Create a cube actor
    Actor* a = new Actor(this);
    a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
    a->SetScale(100.0f);
    MeshComponent* mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("../assets/Cube.gpmesh"));

    // Create a sphere actor
    a = new Actor(this);
    a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
    a->SetScale(3.0f);
    mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("../assets/Sphere.gpmesh"));

    // Set up lighting
    mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir = mRenderer->GetDirectionalLight();
    dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    // Create camera
    mCameraActor = new CameraActor(this);
}
```

---

## 12. Advanced Features

### Step 12.1: Sprite Rendering for UI

Create 2D sprite shaders for UI elements:

`shaders/Sprite.vert`:
```glsl
#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec2 fragTexCoord;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = pos * uWorldTransform * uViewProj;
    
    fragTexCoord = inTexCoord;
}
```

`shaders/Sprite.frag`:
```glsl
#version 330

in vec2 fragTexCoord;
out vec4 outColor;

uniform sampler2D uTexture;

void main() {
    outColor = texture(uTexture, fragTexCoord);
}
```

### Step 12.2: Texture Loading

Implement texture loading for both 3D meshes and UI sprites using a library like SOIL or stb_image.

### Step 12.3: Audio System

Integrate SDL_mixer for sound effects and background music.

### Step 12.4: Collision Detection

Implement bounding sphere or AABB collision detection for game physics.

---

## Summary

This comprehensive guide covers the entire process of creating a 3D game engine from scratch:

1. **Project Setup**: CMake configuration and dependencies
2. **Window Creation**: SDL window and OpenGL context
3. **Renderer**: Modern OpenGL rendering pipeline
4. **Shaders**: Vertex and fragment shader system
5. **Math Library**: Vectors, quaternions, and matrices
6. **Entity-Component System**: Flexible actor and component architecture
7. **Mesh System**: 3D model loading and rendering
8. **Lighting**: Phong lighting model
9. **Camera**: First-person camera with movement
10. **Input**: Keyboard and mouse handling
11. **Game Loop**: Update and render cycles
12. **Advanced Features**: UI, textures, audio, collision

The engine provides a solid foundation for 3D game development with modern rendering techniques, efficient resource management, and extensible architecture.