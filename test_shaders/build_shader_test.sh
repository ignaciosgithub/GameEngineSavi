#!/bin/bash

# Build shader test
g++ -std=c++14 -I.. \
    ShaderTest.cpp \
    ../Model.cpp \
    ../Scene.cpp \
    ../Camera.cpp \
    ../GameObject.cpp \
    ../Vector3.cpp \
    ../Matrix4x4.cpp \
    ../Shaders/Core/Shader.cpp \
    ../Shaders/Core/ShaderProgram.cpp \
    ../Shaders/Core/ShaderError.cpp \
    ../Shaders/Assets/ShaderAsset.cpp \
    -I/usr/include/GL \
    -lGL -lGLEW -lglfw -o shader_test

# Make executable
chmod +x shader_test
