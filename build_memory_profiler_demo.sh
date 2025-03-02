#!/bin/bash
# build_memory_profiler_demo.sh

echo "Building Memory Profiler Demo..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"
INCLUDES="-I."

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile the basic components
echo "Compiling Vector3..."
g++ $CFLAGS $INCLUDES -c Vector3.cpp -o bin/linux/Vector3.o

echo "Compiling Matrix4x4..."
g++ $CFLAGS $INCLUDES -c Matrix4x4.cpp -o bin/linux/Matrix4x4.o

# Compile the profiler
echo "Compiling Profiler..."
g++ $CFLAGS $INCLUDES -c Profiler.cpp -o bin/linux/Profiler.o

# Compile the demo
echo "Compiling MemoryProfilerDemo..."
g++ $CFLAGS $INCLUDES -c MemoryProfilerDemo.cpp -o bin/linux/MemoryProfilerDemo.o

# Link the demo with only the necessary components
echo "Linking MemoryProfilerDemo..."
g++ bin/linux/MemoryProfilerDemo.o bin/linux/Profiler.o bin/linux/Vector3.o bin/linux/Matrix4x4.o -o bin/linux/MemoryProfilerDemo

if [ $? -eq 0 ]; then
    echo "Build successful. Run with: ./bin/linux/MemoryProfilerDemo"
else
    echo "Build failed."
fi
