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

# Find all .cpp files that exist in the repository
EXISTING_CPP_FILES=""
for file in Editor/EditorMain.cpp Scene.cpp GameObject.cpp Vector3.cpp Matrix4x4.cpp Camera.cpp CameraManager.cpp Model.cpp Texture.cpp PointLight.cpp Debugger.cpp FrameCapture.cpp FrameCapture_png.cpp TimeManager.cpp PhysicsSystem.cpp RedundancyDetector.cpp EngineCondition.cpp Graphics/Core/OpenGLGraphicsAPI.cpp Graphics/Core/GraphicsAPIFactory.cpp Shaders/Core/ShaderProgram.cpp Shaders/Core/Shader.cpp Shaders/Core/ShaderError.cpp; do
    if [ -f "$file" ]; then
        EXISTING_CPP_FILES="$EXISTING_CPP_FILES $file"
    fi
done

# Compile the editor
echo "Building editor..."
g++ -o build/editor \
    $EXISTING_CPP_FILES \
    -std=c++11 \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -DGLEW_STATIC \
    -include ThirdParty/OpenGL/include/GL/gl_definitions.h \
    -lGLEW -lGL -lGLU -lglut -lX11

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Editor build successful."
    
    # Check if we're in a headless environment
    if [ -z "$DISPLAY" ]; then
        echo "Warning: No display detected. Running the editor may fail in a headless environment."
        echo "Consider using the emergency editor for headless environments."
    else
        echo "Run the editor with: ./build/editor"
    fi
else
    echo "Editor build failed."
fi
