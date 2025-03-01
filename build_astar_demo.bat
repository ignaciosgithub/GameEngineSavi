@echo off
REM build_astar_demo.bat

echo Building A* Pathfinding Demo...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g

REM Set include paths
set INCLUDES=-I.

REM Set source files
set SOURCES=AStarDemo.cpp NavMesh.cpp NavMeshManager.cpp AIEntity.cpp TimeManager.cpp Vector3.cpp GameObject.cpp Scene.cpp Camera.cpp Model.cpp MonoBehaviourLike.cpp

REM Set output file
set OUTPUT=bin\windows\AStarDemo.exe

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Compile
g++ %CFLAGS% %INCLUDES% %SOURCES% -o %OUTPUT%

echo Build complete.

REM Run the demo
echo Running A* Pathfinding Demo...
%OUTPUT%
