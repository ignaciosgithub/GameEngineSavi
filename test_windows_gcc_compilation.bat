@echo off
echo Testing Windows GCC compilation with MinGW...
echo.

REM Check if mingw32-make is available
where mingw32-make >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: mingw32-make not found. Please install MinGW and add it to your PATH.
    echo You can download MinGW from: https://sourceforge.net/projects/mingw/
    echo.
    echo After installation, add the bin directory to your PATH:
    echo 1. Open System Properties ^> Advanced ^> Environment Variables
    echo 2. Add "C:\MinGW\bin" to Path
    echo.
    pause
    exit /b 1
)

REM Clean any previous build artifacts
echo Cleaning previous build artifacts...
mingw32-make -f Makefile.mingw clean

REM Create the bin/windows directory
if not exist bin\windows mkdir bin\windows

REM Compile the SuperSimplePhysicsDemo_Windows.cpp
echo.
echo Compiling SuperSimplePhysicsDemo_Windows.cpp...
g++ -std=c++11 -Wall -Wextra -I. -c SuperSimplePhysicsDemo_Windows.cpp -o SuperSimplePhysicsDemo_Windows.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile SuperSimplePhysicsDemo_Windows.cpp
    pause
    exit /b 1
)

REM Compile Vector3.cpp
echo.
echo Compiling Vector3.cpp...
g++ -std=c++11 -Wall -Wextra -I. -c Vector3.cpp -o Vector3.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile Vector3.cpp
    pause
    exit /b 1
)

REM Link the SuperSimplePhysicsDemo.exe
echo.
echo Linking SuperSimplePhysicsDemo.exe...
g++ -std=c++11 -Wall -Wextra -I. -o bin\windows\SuperSimplePhysicsDemo.exe SuperSimplePhysicsDemo_Windows.o Vector3.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to link SuperSimplePhysicsDemo.exe
    pause
    exit /b 1
)

REM Compile PhysicsDemo.cpp
echo.
echo Compiling PhysicsDemo.cpp...
g++ -std=c++11 -Wall -Wextra -I. -c PhysicsDemo.cpp -o PhysicsDemo.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile PhysicsDemo.cpp
    pause
    exit /b 1
)

REM Compile CollisionSystem.cpp
echo.
echo Compiling CollisionSystem.cpp...
g++ -std=c++11 -Wall -Wextra -I. -c CollisionSystem.cpp -o CollisionSystem.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile CollisionSystem.cpp
    pause
    exit /b 1
)

REM Compile PhysicsSystem.cpp
echo.
echo Compiling PhysicsSystem.cpp...
g++ -std=c++11 -Wall -Wextra -I. -c PhysicsSystem.cpp -o PhysicsSystem.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile PhysicsSystem.cpp
    pause
    exit /b 1
)

REM Compile RigidBody.cpp
echo.
echo Compiling RigidBody.cpp...
g++ -std=c++11 -Wall -Wextra -I. -c RigidBody.cpp -o RigidBody.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile RigidBody.cpp
    pause
    exit /b 1
)

REM Link the PhysicsDemo.exe
echo.
echo Linking PhysicsDemo.exe...
g++ -std=c++11 -Wall -Wextra -I. -o bin\windows\PhysicsDemo.exe PhysicsDemo.o Vector3.o CollisionSystem.o PhysicsSystem.o RigidBody.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to link PhysicsDemo.exe
    pause
    exit /b 1
)

echo.
echo Compilation successful!
echo.
echo The following executables have been created:
echo - bin\windows\SuperSimplePhysicsDemo.exe
echo - bin\windows\PhysicsDemo.exe
echo.
echo You can run them by double-clicking on them or from the command line.
echo.

pause
