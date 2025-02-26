#!/bin/bash

# Build shader test
g++ -std=c++14 -I.. \
    ShaderTest.cpp \
    ../Model.cpp \
    ../Scene.cpp \
    ../Camera.cpp \
    ../GameObject.cpp \
    ../Shaders/Core/Shader.cpp \
    ../Shaders/Core/ShaderProgram.cpp \
    ../Shaders/Core/ShaderError.cpp \
    ../Shaders/Assets/ShaderAsset.cpp \
    -lGL -lGLEW -o shader_test

# Make executable
chmod +x shader_test
