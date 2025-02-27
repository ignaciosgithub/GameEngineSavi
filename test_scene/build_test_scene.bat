@echo off
echo Building test scene program...

REM Ensure output directories exist
if not exist bin mkdir bin
if not exist bin\windows mkdir bin\windows

REM Build test scene program
g++ -std=c++14 TestSceneObjects.cpp ^
    ..\TriggerVolume.cpp ^
    ..\InvisibleWall.cpp ^
    ..\RigidBody.cpp ^
    ..\PhysicsSystem.cpp ^
    ..\CollisionSystem.cpp ^
    ..\GameObject.cpp ^
    ..\Scene.cpp ^
    ..\Vector3.cpp ^
    ..\Audio\AudioSystem.cpp ^
    ..\Audio\AudioSource.cpp ^
    ..\Audio\AudioClip.cpp ^
    ..\Audio\AudioListener.cpp ^
    -I.. -I..\ThirdParty ^
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lopengl32 -lglu32 ^
    -o test_scene_objects.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run test_scene_objects.exe to test scene objects.
pause
