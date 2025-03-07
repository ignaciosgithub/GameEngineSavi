#!/bin/bash

# Create output directory
mkdir -p bin

# Compile the text controlled editor
g++ -std=c++11 -o bin/text_controlled_editor \
    Editor/TextControlledEditor.cpp \
    Graphics/Core/GraphicsAPIFactory.cpp \
    Graphics/Core/OpenGLGraphicsAPI.cpp \
    TimeManager.cpp \
    Vector3.cpp \
    FrameCapture_png.cpp \
    ThirdParty/stb/stb_image_write_impl.cpp \
    Editor/Editor.cpp \
    Editor/HierarchyPanel.cpp \
    Editor/InspectorPanel.cpp \
    Editor/ProjectPanel.cpp \
    Editor/SceneViewPanel.cpp \
    Scene.cpp \
    Camera.cpp \
    CameraManager.cpp \
    GameObject.cpp \
    Model.cpp \
    Texture.cpp \
    Matrix4x4.cpp \
    MonoBehaviourLike.cpp \
    Shaders/Core/ShaderProgram.cpp \
    Shaders/Core/Shader.cpp \
    Shaders/Core/ShaderError.cpp \
    -I. \
    -DGL_GLEXT_PROTOTYPES \
    -DGLEW_STATIC \
    -lGL -lGLU -lglut -lGLEW -lX11

# Make executable
chmod +x bin/text_controlled_editor

echo "Build complete. Run with ./bin/text_controlled_editor"
