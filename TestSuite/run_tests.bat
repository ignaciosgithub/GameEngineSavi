@echo off
echo Building GameEngineSavi Test Suite...

REM Engine source files needed for tests
set ENGINE_SOURCES=..\Vector3.cpp ..\PhysicsSystem.cpp ..\RigidBody.cpp ..\GameObject.cpp ..\CollisionSystem.cpp ..\Time.cpp ..\Scene.cpp ..\EngineCondition.cpp

REM Compile the test suite with engine sources
g++ -std=c++11 -Wall -I../ src\TestRunner.cpp %ENGINE_SOURCES% -o run_tests.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Running tests...
    run_tests.exe
) else (
    echo Build failed with error code %ERRORLEVEL%
    echo.
    echo This may be due to missing implementation files or compilation errors.
    echo Check that all required engine files exist and are properly implemented.
)
echo.
echo Press any key to exit...
pause > nul
