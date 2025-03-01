@echo off
REM build_nav_mesh_demo.bat

echo Building Navigation Mesh Demo...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g

REM Set include paths
set INCLUDES=-I.

REM Set source files
set SOURCES=NavMeshDemo.cpp NavMesh.cpp NavMeshManager.cpp AIEntity.cpp Raycast.cpp

REM Set output file
set OUTPUT=bin\windows\NavMeshDemo.exe

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Compile
g++ %CFLAGS% %INCLUDES% %SOURCES% -o %OUTPUT%

echo Build complete.

REM Run the demo
echo Running Navigation Mesh Demo...
%OUTPUT%
