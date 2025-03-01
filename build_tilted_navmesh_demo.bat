@echo off
REM build_tilted_navmesh_demo.bat

echo Building Tilted NavMesh Demo...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g

REM Set include paths
set INCLUDES=-I.

REM Set source files
set SOURCES=TiltedNavMeshDemo.cpp Vector3.cpp

REM Set output file
set OUTPUT=bin\windows\TiltedNavMeshDemo.exe

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Compile
g++ %CFLAGS% %INCLUDES% %SOURCES% -o %OUTPUT%

if %ERRORLEVEL% EQU 0 (
    echo Build complete.
    
    REM Run the demo
    echo Running Tilted NavMesh Demo...
    %OUTPUT%
) else (
    echo Build failed.
)
