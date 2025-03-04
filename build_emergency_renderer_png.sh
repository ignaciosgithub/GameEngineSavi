#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the PNG emergency renderer
g++ -std=c++11 -o bin/linux/Editor_emergency_png \
    Editor/Editor_emergency_png.cpp \
    FrameCapture_png.cpp \
    Editor/Editor.cpp \
    Editor/HierarchyPanel.cpp \
    Editor/InspectorPanel.cpp \
    Editor/ProjectPanel.cpp \
    Editor/SceneViewPanel.cpp \
    Editor/Vector3Field.cpp \
    GameObject.cpp \
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
    Editor/TextField.cpp \
    Texture.cpp \
    EngineCondition.cpp \
    ProjectSettings/ProjectSettings.cpp \
    ProjectSettings/ProjectManager.cpp \
    Profiler.cpp \
    Debugger.cpp \
    RedundancyDetector.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -lGL -lGLU -lglut -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "PNG emergency renderer build successful."
    chmod +x bin/linux/Editor_emergency_png
else
    echo "PNG emergency renderer build failed."
fi
