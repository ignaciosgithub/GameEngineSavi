#!/bin/bash

# Check if GLEW is installed
if ! dpkg -l | grep -q libglew-dev; then
    echo "GLEW development package not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y libglew-dev
fi

# Check if GLUT is installed
if ! dpkg -l | grep -q freeglut3-dev; then
    echo "GLUT development package not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y freeglut3-dev
fi

# Create build directory if it doesn't exist
mkdir -p build

# Compile the editor
echo "Building editor..."
g++ -o build/editor \
    Editor/EditorMain.cpp \
    Scene.cpp \
    GameObject.cpp \
    Transform.cpp \
    Vector3.cpp \
    Vector2.cpp \
    Matrix4x4.cpp \
    Quaternion.cpp \
    Camera.cpp \
    CameraManager.cpp \
    Model.cpp \
    Texture.cpp \
    Material.cpp \
    Shader.cpp \
    ShaderProgram.cpp \
    Light.cpp \
    PointLight.cpp \
    DirectionalLight.cpp \
    SpotLight.cpp \
    FrameCapture.cpp \
    FrameCapture_png.cpp \
    Debugger.cpp \
    -std=c++11 \
    -DGLEW_STATIC \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -lGLEW -lGL -lGLU -lglut -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Editor build successful."
    echo "Run the editor with: ./build/editor"
else
    echo "Editor build failed."
fi
