#!/bin/bash
echo "Building simple scene transition test..."

# Create output directory if it doesn't exist
mkdir -p ../bin/linux

g++ -std=c++14 \
    SimpleSceneTransitionTest.cpp \
    -o ../bin/linux/simple_scene_transition_test

# Make executable
chmod +x ../bin/linux/simple_scene_transition_test

echo "Build complete. Run ../bin/linux/simple_scene_transition_test to test scene transitions."
