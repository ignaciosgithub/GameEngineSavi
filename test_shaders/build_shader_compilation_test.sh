#!/bin/bash

# Build shader compilation test
g++ -std=c++14 -I.. \
    ShaderCompilationTest.cpp \
    ../Shaders/Core/Shader.cpp \
    ../Shaders/Core/ShaderProgram.cpp \
    ../Shaders/Core/ShaderError.cpp \
    ../Shaders/Assets/ShaderAsset.cpp \
    -I/usr/include/GL \
    -lGL -lGLEW -lglfw -o shader_compilation_test

# Make executable
chmod +x shader_compilation_test

echo "Build complete. Run ./shader_compilation_test to test shader compilation."
