#!/bin/bash
# build_nav_mesh_demo.sh

echo "Building Navigation Mesh Demo..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"

# Set include paths
INCLUDES="-I."

# Set source files
SOURCES="NavMeshDemo.cpp NavMesh.cpp NavMeshManager.cpp AIEntity.cpp Raycast.cpp"

# Set output file
OUTPUT="bin/linux/NavMeshDemo"

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile
g++ $CFLAGS $INCLUDES $SOURCES -o $OUTPUT

echo "Build complete."

# Run the demo
echo "Running Navigation Mesh Demo..."
$OUTPUT
