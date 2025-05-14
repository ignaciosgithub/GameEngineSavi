#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the standalone emergency red cube renderer
echo "Building standalone emergency red cube renderer..."
g++ -std=c++11 -o bin/linux/EmergencyEditor_standalone \
    Editor/EmergencyEditor_standalone.cpp \
    Vector3.cpp \
    Matrix4x4.cpp \
    TimeManager.cpp \
    Graphics/Core/OpenGLGraphicsAPI.cpp \
    Graphics/Core/GraphicsAPIFactory.cpp \
    Shaders/Core/ShaderProgram.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -DPLATFORM_LINUX \
    -DGLEW_STATIC \
    -lGL -lGLU -lglut -lGLEW -lX11 -ldl -lpthread

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Standalone emergency red cube renderer build successful."
    chmod +x bin/linux/EmergencyEditor_standalone
    
    # Run the standalone emergency red cube renderer
    echo "Running standalone emergency red cube renderer..."
    ./bin/linux/EmergencyEditor_standalone
else
    echo "Standalone emergency red cube renderer build failed."
fi
