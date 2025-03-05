#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Compile the editor with minimal dependencies
echo "Building minimal editor..."
g++ -o build/editor_minimal \
    Editor/EditorMain.cpp \
    Scene_minimal.cpp \
    Vector3.cpp \
    Matrix4x4.cpp \
    Camera.cpp \
    CameraManager.cpp \
    PointLight.cpp \
    -std=c++11 \
    -I. \
    -IThirdParty/OpenGL/include \
    -lGL -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Editor build successful."
    echo "Run the editor with: ./build/editor_minimal"
else
    echo "Editor build failed."
fi
