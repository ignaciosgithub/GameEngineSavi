#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Build the simple torque demo
g++ -std=c++11 -o bin/linux/SimpleTorqueDemo SimpleTorqueDemo.cpp Vector3.cpp -I.

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Running demo..."
    ./bin/linux/SimpleTorqueDemo
else
    echo "Build failed."
fi
