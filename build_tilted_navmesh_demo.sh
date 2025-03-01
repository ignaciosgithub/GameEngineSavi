#!/bin/bash
# build_tilted_navmesh_demo.sh

echo "Building Tilted NavMesh Demo..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"

# Set include paths
INCLUDES="-I."

# Set source files
SOURCES="TiltedNavMeshDemo.cpp Vector3.cpp"

# Set output file
OUTPUT="bin/linux/TiltedNavMeshDemo"

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile
g++ $CFLAGS $INCLUDES $SOURCES -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "Build complete."
    
    # Run the demo
    echo "Running Tilted NavMesh Demo..."
    $OUTPUT
else
    echo "Build failed."
fi
