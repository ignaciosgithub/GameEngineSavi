#!/bin/bash
# build_all_linux_final.sh

echo "Building GameEngineSavi for Linux..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"

# Set include paths
INCLUDES="-I."

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile the engine components in the correct order
echo "Compiling engine components..."

# First, compile the basic components
g++ $CFLAGS $INCLUDES -c Vector3.cpp -o bin/linux/Vector3.o
g++ $CFLAGS $INCLUDES -c Matrix4x4.cpp -o bin/linux/Matrix4x4.o
g++ $CFLAGS $INCLUDES -c TimeManager.cpp -o bin/linux/TimeManager.o

# Then compile the model and rendering components
g++ $CFLAGS $INCLUDES -c Model.cpp -o bin/linux/Model.o

# Compile GameObject and related components
g++ $CFLAGS $INCLUDES -c GameObject.cpp -o bin/linux/GameObject.o
g++ $CFLAGS $INCLUDES -c Camera.cpp -o bin/linux/Camera.o

# Compile physics and collision components
g++ $CFLAGS $INCLUDES -c Raycast.cpp -o bin/linux/Raycast.o
g++ $CFLAGS $INCLUDES -c RigidBody.cpp -o bin/linux/RigidBody.o
g++ $CFLAGS $INCLUDES -c CollisionSystem.cpp -o bin/linux/CollisionSystem.o
g++ $CFLAGS $INCLUDES -c PhysicsSystem.cpp -o bin/linux/PhysicsSystem.o

# Compile navigation mesh components
g++ $CFLAGS $INCLUDES -c NavMesh.cpp -o bin/linux/NavMesh.o
g++ $CFLAGS $INCLUDES -c NavMeshManager.cpp -o bin/linux/NavMeshManager.o
g++ $CFLAGS $INCLUDES -c AIEntity.cpp -o bin/linux/AIEntity.o

# Compile scene and project settings
g++ $CFLAGS $INCLUDES -c Scene.cpp -o bin/linux/Scene.o
g++ $CFLAGS $INCLUDES -c ProjectSettings/ProjectSettings.cpp -o bin/linux/ProjectSettings.o

# Create a static library
echo "Creating static library..."
ar rcs bin/linux/libGameEngineSavi.a bin/linux/Vector3.o bin/linux/Matrix4x4.o bin/linux/Model.o bin/linux/GameObject.o bin/linux/Camera.o bin/linux/Scene.o bin/linux/Raycast.o bin/linux/PhysicsSystem.o bin/linux/CollisionSystem.o bin/linux/TimeManager.o bin/linux/NavMesh.o bin/linux/NavMeshManager.o bin/linux/AIEntity.o bin/linux/ProjectSettings.o bin/linux/RigidBody.o

# Build demos
echo "Building demos..."
if [ -f build_nav_mesh_demo.sh ]; then
    chmod +x build_nav_mesh_demo.sh
    ./build_nav_mesh_demo.sh
fi

if [ -f build_astar_demo.sh ]; then
    chmod +x build_astar_demo.sh
    ./build_astar_demo.sh
fi

if [ -f build_simple_astar_demo.sh ]; then
    chmod +x build_simple_astar_demo.sh
    ./build_simple_astar_demo.sh
fi

if [ -f build_tilted_navmesh_demo.sh ]; then
    chmod +x build_tilted_navmesh_demo.sh
    ./build_tilted_navmesh_demo.sh
fi

if [ $? -eq 0 ]; then
    echo "Build complete. All components built successfully."
else
    echo "Build failed."
fi

# Clean up object files (optional)
# rm -f bin/linux/*.o
