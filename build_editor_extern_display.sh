#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the external display editor
g++ -std=c++11 -o bin/linux/Editor_extern_display \
    Editor/Editor_extern_display_main.cpp \
    Editor/Editor_extern_display.cpp \
    FrameCapture_png.cpp \
    Vector3.cpp \
    Matrix4x4.cpp \
    Camera.cpp \
    Model.cpp \
    MonoBehaviourLike.cpp \
    TimeManager.cpp \
    Raycast.cpp \
    RigidBody.cpp \
    CollisionSystem.cpp \
    PhysicsSystem.cpp \
    Scene.cpp \
    PointLight.cpp \
    CameraManager.cpp \
    Shaders/Core/ShaderProgram.cpp \
    Shaders/Core/Shader.cpp \
    Shaders/Core/ShaderError.cpp \
    Texture.cpp \
    EngineCondition.cpp \
    ProjectSettings/ProjectSettings.cpp \
    ProjectSettings/ProjectManager.cpp \
    Profiler.cpp \
    Debugger.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -lGL -lGLU -lglut -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "External display editor build successful."
    chmod +x bin/linux/Editor_extern_display
    
    # Create frames directory if it doesn't exist
    mkdir -p frames
    
    # Run the external display editor
    echo "Running external display editor to generate frames..."
    ./bin/linux/Editor_extern_display
else
    echo "External display editor build failed."
fi
