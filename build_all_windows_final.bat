@echo off
REM build_all_windows_final.bat

echo Building GameEngineSavi for Windows...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g
set INCLUDES=-I.

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Compile the engine components in the correct order
echo Compiling engine components...

REM First, compile the basic components
g++ %CFLAGS% %INCLUDES% -c Vector3.cpp -o bin\windows\Vector3.o
g++ %CFLAGS% %INCLUDES% -c Matrix4x4.cpp -o bin\windows\Matrix4x4.o
g++ %CFLAGS% %INCLUDES% -c TimeManager.cpp -o bin\windows\TimeManager.o

REM Then compile the model and rendering components
g++ %CFLAGS% %INCLUDES% -c Model.cpp -o bin\windows\Model.o

REM Compile GameObject and related components
g++ %CFLAGS% %INCLUDES% -c GameObject.cpp -o bin\windows\GameObject.o
g++ %CFLAGS% %INCLUDES% -c Camera.cpp -o bin\windows\Camera.o

REM Compile physics and collision components
g++ %CFLAGS% %INCLUDES% -c Raycast.cpp -o bin\windows\Raycast.o
g++ %CFLAGS% %INCLUDES% -c RigidBody.cpp -o bin\windows\RigidBody.o
g++ %CFLAGS% %INCLUDES% -c CollisionSystem.cpp -o bin\windows\CollisionSystem.o
g++ %CFLAGS% %INCLUDES% -c PhysicsSystem.cpp -o bin\windows\PhysicsSystem.o

REM Compile navigation mesh components
g++ %CFLAGS% %INCLUDES% -c NavMesh.cpp -o bin\windows\NavMesh.o
g++ %CFLAGS% %INCLUDES% -c NavMeshManager.cpp -o bin\windows\NavMeshManager.o
g++ %CFLAGS% %INCLUDES% -c AIEntity.cpp -o bin\windows\AIEntity.o

REM Compile scene and project settings
g++ %CFLAGS% %INCLUDES% -c Scene.cpp -o bin\windows\Scene.o
g++ %CFLAGS% %INCLUDES% -c ProjectSettings\ProjectSettings.cpp -o bin\windows\ProjectSettings.o

REM Create a static library
echo Creating static library...
ar rcs bin\windows\libGameEngineSavi.a bin\windows\Vector3.o bin\windows\Matrix4x4.o bin\windows\Model.o bin\windows\GameObject.o bin\windows\Camera.o bin\windows\Scene.o bin\windows\Raycast.o bin\windows\PhysicsSystem.o bin\windows\CollisionSystem.o bin\windows\TimeManager.o bin\windows\NavMesh.o bin\windows\NavMeshManager.o bin\windows\AIEntity.o bin\windows\ProjectSettings.o bin\windows\RigidBody.o

REM Build demos
echo Building demos...
if exist build_nav_mesh_demo.bat (
    call build_nav_mesh_demo.bat
)

if exist build_astar_demo.bat (
    call build_astar_demo.bat
)

if exist build_simple_astar_demo.bat (
    call build_simple_astar_demo.bat
)

if exist build_tilted_navmesh_demo.bat (
    call build_tilted_navmesh_demo.bat
)

if %ERRORLEVEL% == 0 (
    echo Build complete. All components built successfully.
) else (
    echo Build failed.
)

REM Clean up object files (optional)
REM del bin\windows\*.o
