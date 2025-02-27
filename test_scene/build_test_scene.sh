#!/bin/bash

# Build test scene program
echo "Building test scene program..."
g++ -std=c++14 TestSceneObjects.cpp \
    ../TriggerVolume.cpp \
    ../InvisibleWall.cpp \
    ../RigidBody.cpp \
    ../PhysicsSystem.cpp \
    ../CollisionSystem.cpp \
    ../GameObject.cpp \
    ../Scene.cpp \
    ../Vector3.cpp \
    ../Audio/AudioSystem.cpp \
    ../Audio/AudioSource.cpp \
    ../Audio/AudioClip.cpp \
    ../Audio/AudioListener.cpp \
    -I.. -I../ThirdParty \
    -lSDL2 -lSDL2_mixer -lGL -lGLEW -lglfw \
    -o test_scene_objects

# Make executable
chmod +x test_scene_objects

echo "Build complete. Run ./test_scene_objects to test scene objects."
