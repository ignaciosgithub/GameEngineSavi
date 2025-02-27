@echo off
echo Building multi-camera test program...

REM Create output directory if it doesn't exist
if not exist ..\bin\windows mkdir ..\bin\windows

g++ -std=c++14 MultiCameraTest.cpp ^
    ..\Scene.cpp ^
    ..\Camera.cpp ^
    ..\CameraManager.cpp ^
    ..\GameObject.cpp ^
    ..\Vector3.cpp ^
    ..\EngineTime.cpp ^
    ..\PhysicsSystem.cpp ^
    ..\Model.cpp ^
    -I.. -I..\ThirdParty ^
    -lopengl32 -lglu32 -lSDL2 ^
    -o multicamera_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run multicamera_test.exe to test multiple camera views.
pause
