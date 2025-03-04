#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the simplified external display editor
g++ -std=c++11 -o bin/linux/Editor_extern_display_simple \
    Editor/Editor_extern_display_simple.cpp \
    FrameCapture_png.cpp \
    Vector3.cpp \
    Matrix4x4.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -lGL -lGLU -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Simplified external display editor build successful."
    chmod +x bin/linux/Editor_extern_display_simple
    
    # Create frames directory if it doesn't exist
    mkdir -p frames
    
    # Run the simplified external display editor
    echo "Running simplified external display editor to generate frames..."
    ./bin/linux/Editor_extern_display_simple
else
    echo "Simplified external display editor build failed."
fi
