#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the torque demo
g++ -std=c++11 -o bin/linux/TorqueDemo TorqueDemo.cpp RigidBody.cpp GameObject.cpp Vector3.cpp Matrix4x4.cpp MonoBehaviourLike.cpp -I.

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Running demo..."
    ./bin/linux/TorqueDemo
else
    echo "Build failed."
fi
