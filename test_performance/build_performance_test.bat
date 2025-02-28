@echo off
echo Building performance test...

REM Create output directory if it doesn't exist
if not exist ..\bin\windows mkdir ..\bin\windows

g++ -std=c++14 PerformanceTest.cpp ^
    ..\Scene.cpp ^
    ..\GameObject.cpp ^
    ..\PhysicsSystem.cpp ^
    ..\RigidBody.cpp ^
    ..\Vector3.cpp ^
    ..\EngineTime.cpp ^
    ..\Camera.cpp ^
    ..\Model.cpp ^
    ..\CollisionSystem.cpp ^
    -I.. ^
    -lopengl32 -lglu32 ^
    -o performance_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run performance_test.exe to test performance.
echo.
echo Press any key to run the performance test...
pause > nul
performance_test.exe
