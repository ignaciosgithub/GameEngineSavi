#!/bin/bash
echo "Building scene transition test..."

# Create output directory if it doesn't exist
mkdir -p ../bin/linux

g++ -std=c++14 \
    SceneTransitionTest.cpp \
    ../Scene.cpp \
    ../GameObject.cpp \
    ../Vector3.cpp \
    ../EngineTime.cpp \
    ../Matrix4x4.cpp \
    ../PhysicsSystem.cpp \
    ../CollisionSystem.cpp \
    ../RigidBody.cpp \
    ../SceneSerializer.cpp \
    ../CameraManager.cpp \
    ../Camera.cpp \
    ../PointLight.cpp \
    -I.. \
    -o ../bin/linux/scene_transition_test

# Make executable
chmod +x ../bin/linux/scene_transition_test

echo "Build complete. Run ../bin/linux/scene_transition_test to test scene transitions."
