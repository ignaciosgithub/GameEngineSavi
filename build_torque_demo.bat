@echo off

REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Build the torque demo
g++ -std=c++11 -o bin\windows\TorqueDemo.exe TorqueDemo.cpp RigidBody.cpp GameObject.cpp Vector3.cpp Matrix4x4.cpp MonoBehaviourLike.cpp -I.

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Build successful. Running demo...
    bin\windows\TorqueDemo.exe
) else (
    echo Build failed.
)
