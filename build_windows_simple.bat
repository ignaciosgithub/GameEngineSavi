@echo off
echo Building Super Simple Physics Demo for Windows...

REM Check if Visual Studio is installed
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Visual Studio compiler not found.
    echo Please run this script from a Visual Studio Developer Command Prompt.
    exit /b 1
)

REM Set compiler flags
set CFLAGS=/EHsc /std:c++14 /DPLATFORM_WINDOWS

REM Create output directory
if not exist bin\windows mkdir bin\windows

REM Compile source files
cl %CFLAGS% /c Vector3.cpp
cl %CFLAGS% /c SuperSimplePhysicsDemo_Windows.cpp

REM Link object files
link Vector3.obj SuperSimplePhysicsDemo_Windows.obj /OUT:bin\windows\SuperSimplePhysicsDemo.exe

echo Build complete.
if exist bin\windows\SuperSimplePhysicsDemo.exe (
    echo SuperSimplePhysicsDemo.exe created successfully in bin\windows directory.
) else (
    echo Build failed.
)

exit /b 0
