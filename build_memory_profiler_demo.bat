@echo off
REM build_memory_profiler_demo.bat

echo Building Memory Profiler Demo...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g
set INCLUDES=-I.

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Compile the profiler
g++ %CFLAGS% %INCLUDES% -c Profiler.cpp -o bin\windows\Profiler.o

REM Compile the demo
g++ %CFLAGS% %INCLUDES% -c MemoryProfilerDemo.cpp -o bin\windows\MemoryProfilerDemo.o

REM Link the demo with the engine components
g++ bin\windows\MemoryProfilerDemo.o bin\windows\Profiler.o bin\windows\Vector3.o bin\windows\Matrix4x4.o bin\windows\Model.o bin\windows\GameObject.o bin\windows\Camera.o bin\windows\Scene.o bin\windows\Raycast.o bin\windows\PhysicsSystem.o bin\windows\CollisionSystem.o bin\windows\TimeManager.o bin\windows\NavMesh.o bin\windows\NavMeshManager.o bin\windows\AIEntity.o bin\windows\ProjectSettings.o bin\windows\RigidBody.o -o bin\windows\MemoryProfilerDemo.exe

if %ERRORLEVEL% == 0 (
    echo Build successful. Run with: bin\windows\MemoryProfilerDemo.exe
) else (
    echo Build failed.
)
