#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the emergency red cube renderer
echo "Building emergency red cube renderer..."
g++ -std=c++11 -o bin/linux/EmergencyEditor_red_cube \
    Editor/EmergencyEditor_red_cube.cpp \
    Vector3.cpp \
    Matrix4x4.cpp \
    TimeManager.cpp \
    Graphics/Core/OpenGLGraphicsAPI.cpp \
    Graphics/Core/GraphicsAPIFactory.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -DPLATFORM_LINUX \
    -lGL -lGLU -lglut -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Emergency red cube renderer build successful."
    chmod +x bin/linux/EmergencyEditor_red_cube
else
    echo "Emergency red cube renderer build failed."
fi
