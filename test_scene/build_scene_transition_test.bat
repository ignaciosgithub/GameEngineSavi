@echo off
echo Building scene transition test...

REM Create output directory if it doesn't exist
if not exist ..\bin\windows mkdir ..\bin\windows

g++ -std=c++14 ^
    SceneTransitionTest.cpp ^
    ..\Scene.cpp ^
    ..\GameObject.cpp ^
    ..\Vector3.cpp ^
    ..\EngineTime.cpp ^
    ..\Matrix4x4.cpp ^
    ..\PhysicsSystem.cpp ^
    ..\CollisionSystem.cpp ^
    ..\RigidBody.cpp ^
    ..\SceneSerializer.cpp ^
    ..\CameraManager.cpp ^
    ..\Camera.cpp ^
    ..\PointLight.cpp ^
    -I.. ^
    -o ..\bin\windows\scene_transition_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run ..\bin\windows\scene_transition_test.exe to test scene transitions.
echo.
echo Press any key to run the test...
pause > nul
..\bin\windows\scene_transition_test.exe
