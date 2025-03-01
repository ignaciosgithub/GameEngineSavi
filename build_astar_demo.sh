#!/bin/bash
# build_astar_demo.sh

echo "Building A* Pathfinding Demo..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"

# Set include paths
INCLUDES="-I."

# Set source files
SOURCES="AStarDemo.cpp NavMesh.cpp NavMeshManager.cpp AIEntity.cpp TimeManager.cpp Vector3.cpp GameObject.cpp Scene.cpp Camera.cpp Model.cpp MonoBehaviourLike.cpp"

# Set output file
OUTPUT="bin/linux/AStarDemo"

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile
g++ $CFLAGS $INCLUDES $SOURCES -o $OUTPUT

echo "Build complete."

# Run the demo
echo "Running A* Pathfinding Demo..."
$OUTPUT
