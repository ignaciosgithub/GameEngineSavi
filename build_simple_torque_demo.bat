@echo off

REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Build the simple torque demo
g++ -std=c++11 -o bin\windows\SimpleTorqueDemo.exe SimpleTorqueDemo.cpp Vector3.cpp -I.

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Build successful. Running demo...
    bin\windows\SimpleTorqueDemo.exe
) else (
    echo Build failed.
)
