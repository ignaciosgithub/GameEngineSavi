#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the main editor window
echo "Building main editor window..."
g++ -std=c++11 -o bin/linux/main_editor_window \
    main_editor_window.cpp \
    Vector3.cpp \
    Matrix4x4.cpp \
    TimeManager.cpp \
    Graphics/Core/OpenGLGraphicsAPI.cpp \
    Graphics/Core/GraphicsAPIFactory.cpp \
    Editor/Editor.cpp \
    Editor/HierarchyPanel.cpp \
    Editor/InspectorPanel.cpp \
    Editor/ProjectPanel.cpp \
    Editor/SceneViewPanel.cpp \
    Scene.cpp \
    GameObject.cpp \
    Camera.cpp \
    PhysicsSystem.cpp \
    CameraManager.cpp \
    Model.cpp \
    EngineCondition.cpp \
    Shaders/Core/ShaderProgram.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -DPLATFORM_LINUX \
    -DGLEW_STATIC \
    -lGL -lGLU -lglut -lX11 -lGLEW

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Main editor window build successful."
    chmod +x bin/linux/main_editor_window
    
    # Run the main editor window
    echo "Running main editor window..."
    ./bin/linux/main_editor_window
else
    echo "Main editor window build failed."
fi
