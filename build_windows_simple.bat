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

REM Compile source files
cl %CFLAGS% /c Vector3.cpp
cl %CFLAGS% /c SuperSimplePhysicsDemo.cpp

REM Link object files
link Vector3.obj SuperSimplePhysicsDemo.obj /OUT:SuperSimplePhysicsDemo.exe

echo Build complete.
if exist SuperSimplePhysicsDemo.exe (
    echo SuperSimplePhysicsDemo.exe created successfully.
) else (
    echo Build failed.
)

exit /b 0
