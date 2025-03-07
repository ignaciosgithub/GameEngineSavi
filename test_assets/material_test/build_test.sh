#!/bin/bash

# Create output directory
mkdir -p bin

# Compile test program
g++ -std=c++11 -o bin/test_obj_loader \
    test_obj_loader.cpp \
    ../../Model.cpp \
    ../../Vector3.cpp \
    ../../Matrix4x4.cpp \
    ../../Graphics/Core/GraphicsAPIFactory.cpp \
    ../../Graphics/Core/OpenGLGraphicsAPI.cpp \
    ../../Texture.cpp \
    ../../MonoBehaviourLike.cpp \
    ../../Shaders/Core/ShaderProgram.cpp \
    ../../Shaders/Core/Shader.cpp \
    ../../Shaders/Core/ShaderError.cpp \
    -I../.. \
    -DGL_GLEXT_PROTOTYPES \
    -DGLEW_STATIC \
    -lGL -lGLU -lglut -lGLEW

# Make executable
chmod +x bin/test_obj_loader

echo "Build complete. Run with ./bin/test_obj_loader"
