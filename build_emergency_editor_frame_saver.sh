#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Create frames directory if it doesn't exist
mkdir -p frames

# Build the emergency editor with frame saving functionality
g++ -std=c++11 -o bin/linux/EmergencyEditor_frame_saver \
    Editor/EmergencyEditor_frame_saver.cpp \
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
    Graphics/Core/GraphicsAPIFactory.cpp \
    Graphics/Core/OpenGLGraphicsAPI.cpp \
    ThirdParty/stb/stb_image_write_impl.cpp \
    -I. \
    -IThirdParty/OpenGL/include \
    -DGL_GLEXT_PROTOTYPES \
    -DGLEW_STATIC \
    -lGL -lGLU -lglut -lX11 -lGLEW

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Emergency editor with frame saving functionality build successful."
    chmod +x bin/linux/EmergencyEditor_frame_saver
    
    # Run the emergency editor to generate frames
    echo "Running emergency editor to generate frames..."
    ./bin/linux/EmergencyEditor_frame_saver
    
    # Check if frame4.png exists
    if [ -f "frames/frame4.png" ]; then
        echo "Frame 4 generated successfully."
        echo "You can view frame4.png in the frames directory."
    else
        echo "Frame 4 was not generated. Check the emergency editor output for errors."
    fi
else
    echo "Emergency editor build failed."
fi
