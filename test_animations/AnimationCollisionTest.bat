@echo off
echo Running Animation Collision Test...
echo.

REM Create bin\windows directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Set compiler flags
set CXXFLAGS=-std=c++14 -I. -IThirdParty -DPLATFORM_WINDOWS

REM Build the animation collision test
g++ %CXXFLAGS% ^
    test_animations\AnimationCollisionTest.cpp ^
    Animation\KeyFrame.cpp ^
    Animation\Animation.cpp ^
    Animation\AnimationComponent.cpp ^
    Animation\AnimationLoader.cpp ^
    Model.cpp ^
    Vector3.cpp ^
    CollisionSystem.cpp ^
    RigidBody.cpp ^
    GameObject.cpp ^
    Scene.cpp ^
    PhysicsSystem.cpp ^
    -o bin\windows\AnimationCollisionTest.exe

REM Check if build was successful
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

REM Run the animation collision test
bin\windows\AnimationCollisionTest.exe
pause
