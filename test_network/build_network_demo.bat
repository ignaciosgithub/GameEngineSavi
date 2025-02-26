@echo off
echo Building Network Demo...

REM Create bin/windows directory if it doesn't exist
if not exist ..\bin\windows mkdir ..\bin\windows

REM Set compiler flags
set CXXFLAGS=-std=c++14 -I.. -I..\ThirdParty -DPLATFORM_WINDOWS

REM Build the network demo
g++ %CXXFLAGS% ^
    NetworkDemo.cpp ^
    ..\Network\NetworkSystem.cpp ^
    ..\Network\Connection.cpp ^
    ..\Network\Packet.cpp ^
    ..\Network\NetworkComponent.cpp ^
    ..\Network\NetworkDebugger.cpp ^
    ..\Vector3.cpp ^
    ..\Scene.cpp ^
    ..\EngineCondition.cpp ^
    ..\ProjectSettings\ProjectSettings.cpp ^
    -o ..\bin\windows\NetworkDemo.exe

REM Check if build was successful
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed!
    exit /b 1
)

echo.
echo Build successful! Network demo is available at ..\bin\windows\NetworkDemo.exe
echo Run with: ..\bin\windows\NetworkDemo.exe [--server^|--p2p]
