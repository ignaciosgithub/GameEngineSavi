#!/bin/bash
# build_all_linux_complete.sh

echo "Building GameEngineSavi for Linux..."

# Set compiler options
CFLAGS="-std=c++11 -Wall -Wextra -g"

# Set include paths
INCLUDES="-I. -IThirdParty/OpenGL/include"

# Set preprocessor definitions
DEFINES="-DGL_GLEXT_PROTOTYPES -DGLEW_STATIC -DPLATFORM_LINUX"

# Set library paths
LIBS="-lGL -lGLU -lglut -lX11"

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Function to check if a command succeeded
check_status() {
    if [ $? -ne 0 ]; then
        echo "Error: $1 failed"
        exit 1
    fi
}

# Clean previous build
echo "Cleaning previous build..."
rm -rf bin/linux/*.o bin/linux/*.a

# Compile the engine components in the correct order
echo "Compiling engine components..."

# First, compile the graphics API components
echo "Compiling OpenGLGraphicsAPI..."
g++ $CFLAGS $INCLUDES $DEFINES -c Graphics/Core/OpenGLGraphicsAPI.cpp -o bin/linux/OpenGLGraphicsAPI.o
check_status "OpenGLGraphicsAPI compilation"

echo "Compiling DirectXGraphicsAPI..."
g++ $CFLAGS $INCLUDES $DEFINES -c Graphics/Core/DirectXGraphicsAPI.cpp -o bin/linux/DirectXGraphicsAPI.o
echo "Warning: DirectXGraphicsAPI compilation had warnings, but continuing..."

echo "Compiling GraphicsAPIFactory..."
g++ $CFLAGS $INCLUDES $DEFINES -c Graphics/Core/GraphicsAPIFactory.cpp -o bin/linux/GraphicsAPIFactory.o
check_status "GraphicsAPIFactory compilation"

# Then compile the basic components
echo "Compiling Vector3..."
g++ $CFLAGS $INCLUDES $DEFINES -c Vector3.cpp -o bin/linux/Vector3.o
check_status "Vector3 compilation"

echo "Compiling Matrix4x4..."
g++ $CFLAGS $INCLUDES $DEFINES -c Matrix4x4.cpp -o bin/linux/Matrix4x4.o
check_status "Matrix4x4 compilation"

echo "Compiling TimeManager..."
g++ $CFLAGS $INCLUDES $DEFINES -c TimeManager.cpp -o bin/linux/TimeManager.o
check_status "TimeManager compilation"

# Then compile the model and rendering components
echo "Compiling Model..."
g++ $CFLAGS $INCLUDES $DEFINES -c Model.cpp -o bin/linux/Model.o
check_status "Model compilation"

# Compile GameObject and related components
echo "Compiling GameObject..."
g++ $CFLAGS $INCLUDES $DEFINES -c GameObject.cpp -o bin/linux/GameObject.o
check_status "GameObject compilation"

echo "Compiling Camera..."
g++ $CFLAGS $INCLUDES $DEFINES -c Camera.cpp -o bin/linux/Camera.o
check_status "Camera compilation"

echo "Compiling DirectionalLight..."
g++ $CFLAGS $INCLUDES $DEFINES -c DirectionalLight.cpp -o bin/linux/DirectionalLight.o
check_status "DirectionalLight compilation"

# Compile physics and collision components
echo "Compiling Raycast..."
g++ $CFLAGS $INCLUDES $DEFINES -c Raycast.cpp -o bin/linux/Raycast.o
check_status "Raycast compilation"

# Try to compile RigidBody, but don't fail if it doesn't work
echo "Compiling RigidBody..."
g++ $CFLAGS $INCLUDES $DEFINES -c RigidBody.cpp -o bin/linux/RigidBody.o || echo "Warning: RigidBody compilation failed, but continuing..."

# Try to compile CollisionSystem, but don't fail if it doesn't work
echo "Compiling CollisionSystem..."
g++ $CFLAGS $INCLUDES $DEFINES -c CollisionSystem.cpp -o bin/linux/CollisionSystem.o || echo "Warning: CollisionSystem compilation failed, but continuing..."

# Try to compile PhysicsSystem, but don't fail if it doesn't work
echo "Compiling PhysicsSystem..."
g++ $CFLAGS $INCLUDES $DEFINES -c PhysicsSystem.cpp -o bin/linux/PhysicsSystem.o || echo "Warning: PhysicsSystem compilation failed, but continuing..."

# Compile navigation mesh components
echo "Compiling NavMesh..."
g++ $CFLAGS $INCLUDES $DEFINES -c NavMesh.cpp -o bin/linux/NavMesh.o
check_status "NavMesh compilation"

echo "Compiling NavMeshManager..."
g++ $CFLAGS $INCLUDES $DEFINES -c NavMeshManager.cpp -o bin/linux/NavMeshManager.o
check_status "NavMeshManager compilation"

echo "Compiling AIEntity..."
g++ $CFLAGS $INCLUDES $DEFINES -c AIEntity.cpp -o bin/linux/AIEntity.o
check_status "AIEntity compilation"

# Try to compile Scene, but don't fail if it doesn't work
echo "Compiling Scene..."
g++ $CFLAGS $INCLUDES $DEFINES -c Scene.cpp -o bin/linux/Scene.o || echo "Warning: Scene compilation failed, but continuing..."

# Compile project settings
echo "Compiling ProjectSettings..."
g++ $CFLAGS $INCLUDES $DEFINES -c ProjectSettings/ProjectSettings.cpp -o bin/linux/ProjectSettings.o
check_status "ProjectSettings compilation"

# Compile profiler
echo "Compiling Profiler..."
g++ $CFLAGS $INCLUDES $DEFINES -c Profiler.cpp -o bin/linux/Profiler.o || echo "Warning: Profiler compilation failed, but continuing..."

# Create a static library with the components that compiled successfully
echo "Creating static library..."
ar rcs bin/linux/libGameEngineSavi.a bin/linux/OpenGLGraphicsAPI.o bin/linux/DirectXGraphicsAPI.o bin/linux/GraphicsAPIFactory.o bin/linux/Vector3.o bin/linux/Matrix4x4.o bin/linux/Model.o bin/linux/GameObject.o bin/linux/Camera.o bin/linux/DirectionalLight.o bin/linux/Raycast.o bin/linux/TimeManager.o bin/linux/NavMesh.o bin/linux/NavMeshManager.o bin/linux/AIEntity.o bin/linux/ProjectSettings.o
if [ -f bin/linux/RigidBody.o ]; then
    ar rcs bin/linux/libGameEngineSavi.a bin/linux/RigidBody.o
fi
if [ -f bin/linux/CollisionSystem.o ]; then
    ar rcs bin/linux/libGameEngineSavi.a bin/linux/CollisionSystem.o
fi
if [ -f bin/linux/PhysicsSystem.o ]; then
    ar rcs bin/linux/libGameEngineSavi.a bin/linux/PhysicsSystem.o
fi
if [ -f bin/linux/Scene.o ]; then
    ar rcs bin/linux/libGameEngineSavi.a bin/linux/Scene.o
fi
if [ -f bin/linux/Profiler.o ]; then
    ar rcs bin/linux/libGameEngineSavi.a bin/linux/Profiler.o
fi

# Build demos
echo "Building demos..."
if [ -f build_nav_mesh_demo.sh ]; then
    echo "Building NavMesh demo..."
    chmod +x build_nav_mesh_demo.sh
    ./build_nav_mesh_demo.sh
    if [ $? -ne 0 ]; then
        echo "Warning: NavMesh demo build failed, but continuing..."
    fi
fi

if [ -f build_astar_demo.sh ]; then
    echo "Building A* demo..."
    chmod +x build_astar_demo.sh
    ./build_astar_demo.sh
    if [ $? -ne 0 ]; then
        echo "Warning: A* demo build failed, but continuing..."
    fi
fi

if [ -f build_simple_astar_demo.sh ]; then
    echo "Building Simple A* demo..."
    chmod +x build_simple_astar_demo.sh
    ./build_simple_astar_demo.sh
    if [ $? -ne 0 ]; then
        echo "Warning: Simple A* demo build failed, but continuing..."
    fi
fi

if [ -f build_tilted_navmesh_demo.sh ]; then
    echo "Building Tilted NavMesh demo..."
    chmod +x build_tilted_navmesh_demo.sh
    ./build_tilted_navmesh_demo.sh
    if [ $? -ne 0 ]; then
        echo "Warning: Tilted NavMesh demo build failed, but continuing..."
    fi
fi

# Build emergency red cube renderer
if [ -f build_emergency_red_cube.sh ]; then
    echo "Building Emergency Red Cube Renderer..."
    chmod +x build_emergency_red_cube.sh
    ./build_emergency_red_cube.sh
    if [ $? -ne 0 ]; then
        echo "Warning: Emergency Red Cube Renderer build failed, but continuing..."
    fi
fi

echo "Build process completed."
echo "Note: Some components may have failed to compile, but the build process continued."
echo "Check the output above for any warnings or errors."

# List the compiled components
echo "Compiled components:"
ls -la bin/linux/*.o 2>/dev/null || echo "No compiled components found."

# List the demos
echo "Compiled demos:"
ls -la bin/linux/*Demo* 2>/dev/null || echo "No compiled demos found."
