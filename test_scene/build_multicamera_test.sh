#!/bin/bash

# Build multi-camera test program
echo "Building multi-camera test program..."

# Create output directory if it doesn't exist
mkdir -p ../bin/linux

g++ -std=c++14 MultiCameraTest.cpp \
    ../Scene.cpp \
    ../Camera.cpp \
    ../CameraManager.cpp \
    ../GameObject.cpp \
    ../Vector3.cpp \
    ../EngineTime.cpp \
    ../PhysicsSystem.cpp \
    ../Model.cpp \
    -I.. -I../ThirdParty \
    -lGL -lGLU -lSDL2 \
    -o multicamera_test

# Make executable
chmod +x multicamera_test

echo "Build complete. Run ./multicamera_test to test multiple camera views."
