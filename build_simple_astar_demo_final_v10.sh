#!/bin/bash
# build_simple_astar_demo_final_v10.sh

echo "Building Simple A* Pathfinding Demo (Final v10)..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"

# Set include paths
INCLUDES="-I."

# Set source files
SOURCES="SimpleAStarDemo_final_v10.cpp Vector3.cpp"

# Set output file
OUTPUT="bin/linux/SimpleAStarDemo_final_v10"

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile
g++ $CFLAGS $INCLUDES $SOURCES -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "Build complete."
    
    # Run the demo
    echo "Running Simple A* Pathfinding Demo (Final v10)..."
    $OUTPUT
else
    echo "Build failed."
fi
