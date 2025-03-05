#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Create frames directory if it doesn't exist
mkdir -p frames

# Build the enhanced emergency editor
g++ -std=c++11 -o bin/linux/EnhancedEmergencyEditor \
    Editor/EnhancedEmergencyEditor.cpp \
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
    FrameCapture_png.cpp \
    ProjectSettings/ProjectSettings.cpp \
    ProjectSettings/ProjectManager.cpp \
    Profiler.cpp \
    Debugger.cpp \
    RedundancyDetector.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -DGLEW_STATIC \
    -lGL -lGLU -lglut -lX11 -lGLEW

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Enhanced emergency editor build successful."
    chmod +x bin/linux/EnhancedEmergencyEditor
    
    # Check if we're in a headless environment
    if [ -z "$DISPLAY" ]; then
        echo "Warning: No display detected. Running the editor may fail in a headless environment."
        echo "You can still run the editor manually with: ./bin/linux/EnhancedEmergencyEditor"
    else
        echo "To run the enhanced emergency editor and generate frames, use:"
        echo "./bin/linux/EnhancedEmergencyEditor"
    fi
else
    echo "Enhanced emergency editor build failed."
fi
