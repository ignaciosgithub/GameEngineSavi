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

REM Compile source files
cl %CFLAGS% /c PhysicsSystem.cpp
cl %CFLAGS% /c CollisionSystem.cpp
cl %CFLAGS% /c RigidBody.cpp
cl %CFLAGS% /c Vector3.cpp
cl %CFLAGS% /c PhysicsDemo.cpp

REM Link object files
link PhysicsSystem.obj CollisionSystem.obj RigidBody.obj Vector3.obj PhysicsDemo.obj /OUT:PhysicsDemo.exe opengl32.lib glu32.lib user32.lib gdi32.lib

echo Build complete.
if exist PhysicsDemo.exe (
    echo PhysicsDemo.exe created successfully.
) else (
    echo Build failed.
)

exit /b 0
