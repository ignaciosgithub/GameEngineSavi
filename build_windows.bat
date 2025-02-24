@echo off
echo Building Physics Demo for Windows...

REM Check if Visual Studio is installed
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Visual Studio compiler not found.
    echo Please run this script from a Visual Studio Developer Command Prompt.
    exit /b 1
)

REM Set compiler flags
set CFLAGS=/EHsc /std:c++14 /DPLATFORM_WINDOWS

REM Create output directory
if not exist bin\windows mkdir bin\windows

REM Compile source files
cl %CFLAGS% /c Vector3.cpp
cl %CFLAGS% /c PhysicsSystem.cpp
cl %CFLAGS% /c CollisionSystem.cpp
cl %CFLAGS% /c RigidBody.cpp
cl %CFLAGS% /c PhysicsDemo.cpp

REM Link object files
link Vector3.obj PhysicsSystem.obj CollisionSystem.obj RigidBody.obj PhysicsDemo.obj /OUT:bin\windows\PhysicsDemo.exe opengl32.lib glu32.lib user32.lib gdi32.lib

echo Build complete.
if exist bin\windows\PhysicsDemo.exe (
    echo PhysicsDemo.exe created successfully in bin\windows directory.
) else (
    echo Build failed.
)

exit /b 0
