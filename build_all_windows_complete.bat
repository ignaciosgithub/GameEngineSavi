@echo off
REM build_all_windows_complete.bat

echo Building GameEngineSavi for Windows...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g
set INCLUDES=-I. -IThirdParty/OpenGL/include

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Clean previous build
echo Cleaning previous build...
if exist bin\windows\*.o del /Q bin\windows\*.o
if exist bin\windows\*.a del /Q bin\windows\*.a

REM Compile the engine components in the correct order
echo Compiling engine components...

REM First, compile the basic components
echo Compiling Vector3...
g++ %CFLAGS% %INCLUDES% -c Vector3.cpp -o bin\windows\Vector3.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: Vector3 compilation failed
    exit /b 1
)

echo Compiling Matrix4x4...
g++ %CFLAGS% %INCLUDES% -c Matrix4x4.cpp -o bin\windows\Matrix4x4.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: Matrix4x4 compilation failed
    exit /b 1
)

echo Compiling TimeManager...
g++ %CFLAGS% %INCLUDES% -c TimeManager.cpp -o bin\windows\TimeManager.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: TimeManager compilation failed
    exit /b 1
)

REM Then compile the model and rendering components
echo Compiling Model...
g++ %CFLAGS% %INCLUDES% -c Model.cpp -o bin\windows\Model.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: Model compilation failed
    exit /b 1
)

REM Compile GameObject and related components
echo Compiling GameObject...
g++ %CFLAGS% %INCLUDES% -c GameObject.cpp -o bin\windows\GameObject.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: GameObject compilation failed
    exit /b 1
)

echo Compiling Camera...
g++ %CFLAGS% %INCLUDES% -c Camera.cpp -o bin\windows\Camera.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: Camera compilation failed
    exit /b 1
)

REM Compile physics and collision components
echo Compiling Raycast...
g++ %CFLAGS% %INCLUDES% -c Raycast.cpp -o bin\windows\Raycast.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: Raycast compilation failed
    exit /b 1
)

REM Try to compile RigidBody, but don't fail if it doesn't work
echo Compiling RigidBody...
g++ %CFLAGS% %INCLUDES% -c RigidBody.cpp -o bin\windows\RigidBody.o
if %ERRORLEVEL% NEQ 0 (
    echo Warning: RigidBody compilation failed, but continuing...
)

REM Try to compile CollisionSystem, but don't fail if it doesn't work
echo Compiling CollisionSystem...
g++ %CFLAGS% %INCLUDES% -c CollisionSystem.cpp -o bin\windows\CollisionSystem.o
if %ERRORLEVEL% NEQ 0 (
    echo Warning: CollisionSystem compilation failed, but continuing...
)

REM Try to compile PhysicsSystem, but don't fail if it doesn't work
echo Compiling PhysicsSystem...
g++ %CFLAGS% %INCLUDES% -c PhysicsSystem.cpp -o bin\windows\PhysicsSystem.o
if %ERRORLEVEL% NEQ 0 (
    echo Warning: PhysicsSystem compilation failed, but continuing...
)

REM Compile navigation mesh components
echo Compiling NavMesh...
g++ %CFLAGS% %INCLUDES% -c NavMesh.cpp -o bin\windows\NavMesh.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: NavMesh compilation failed
    exit /b 1
)

echo Compiling NavMeshManager...
g++ %CFLAGS% %INCLUDES% -c NavMeshManager.cpp -o bin\windows\NavMeshManager.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: NavMeshManager compilation failed
    exit /b 1
)

echo Compiling AIEntity...
g++ %CFLAGS% %INCLUDES% -c AIEntity.cpp -o bin\windows\AIEntity.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: AIEntity compilation failed
    exit /b 1
)

REM Try to compile Scene, but don't fail if it doesn't work
echo Compiling Scene...
g++ %CFLAGS% %INCLUDES% -c Scene.cpp -o bin\windows\Scene.o
if %ERRORLEVEL% NEQ 0 (
    echo Warning: Scene compilation failed, but continuing...
)

REM Compile project settings
echo Compiling ProjectSettings...
g++ %CFLAGS% %INCLUDES% -c ProjectSettings\ProjectSettings.cpp -o bin\windows\ProjectSettings.o
if %ERRORLEVEL% NEQ 0 (
    echo Error: ProjectSettings compilation failed
    exit /b 1
)

REM Compile profiler
echo Compiling Profiler...
g++ %CFLAGS% %INCLUDES% -c Profiler.cpp -o bin\windows\Profiler.o
if %ERRORLEVEL% NEQ 0 (
    echo Warning: Profiler compilation failed, but continuing...
)

REM Create a static library with the components that compiled successfully
echo Creating static library...
ar rcs bin\windows\libGameEngineSavi.a bin\windows\Vector3.o bin\windows\Matrix4x4.o bin\windows\Model.o bin\windows\GameObject.o bin\windows\Camera.o bin\windows\Raycast.o bin\windows\TimeManager.o bin\windows\NavMesh.o bin\windows\NavMeshManager.o bin\windows\AIEntity.o bin\windows\ProjectSettings.o

REM Add optional components if they compiled successfully
if exist bin\windows\RigidBody.o (
    ar rcs bin\windows\libGameEngineSavi.a bin\windows\RigidBody.o
)
if exist bin\windows\CollisionSystem.o (
    ar rcs bin\windows\libGameEngineSavi.a bin\windows\CollisionSystem.o
)
if exist bin\windows\PhysicsSystem.o (
    ar rcs bin\windows\libGameEngineSavi.a bin\windows\PhysicsSystem.o
)
if exist bin\windows\Scene.o (
    ar rcs bin\windows\libGameEngineSavi.a bin\windows\Scene.o
)
if exist bin\windows\Profiler.o (
    ar rcs bin\windows\libGameEngineSavi.a bin\windows\Profiler.o
)

REM Build demos
echo Building demos...
if exist build_nav_mesh_demo.bat (
    echo Building NavMesh demo...
    call build_nav_mesh_demo.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Warning: NavMesh demo build failed, but continuing...
    )
)

if exist build_astar_demo.bat (
    echo Building A* demo...
    call build_astar_demo.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Warning: A* demo build failed, but continuing...
    )
)

if exist build_simple_astar_demo.bat (
    echo Building Simple A* demo...
    call build_simple_astar_demo.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Warning: Simple A* demo build failed, but continuing...
    )
)

if exist build_tilted_navmesh_demo.bat (
    echo Building Tilted NavMesh demo...
    call build_tilted_navmesh_demo.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Warning: Tilted NavMesh demo build failed, but continuing...
    )
)

echo Build process completed.
echo Note: Some components may have failed to compile, but the build process continued.
echo Check the output above for any warnings or errors.

REM List the compiled components
echo Compiled components:
dir /B bin\windows\*.o 2>nul || echo No compiled components found.

REM List the demos
echo Compiled demos:
dir /B bin\windows\*Demo*.exe 2>nul || echo No compiled demos found.
