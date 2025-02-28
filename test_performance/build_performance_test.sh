#!/bin/bash

# Build performance test
echo "Building performance test..."

# Create output directory if it doesn't exist
mkdir -p ../bin/linux

g++ -std=c++14 PerformanceTest.cpp \
    ../Scene.cpp \
    ../GameObject.cpp \
    ../PhysicsSystem.cpp \
    ../RigidBody.cpp \
    ../Vector3.cpp \
    ../EngineTime.cpp \
    ../Camera.cpp \
    ../Model.cpp \
    ../CollisionSystem.cpp \
    -I.. \
    -o performance_test

# Make executable
chmod +x performance_test

echo "Build complete. Run ./performance_test to test performance."
