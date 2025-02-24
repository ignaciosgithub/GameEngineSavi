@echo off
echo ===================================================
echo Building Super Simple Physics Demo for Windows...
echo ===================================================

REM Check if Visual Studio is installed
echo Checking for Visual Studio compiler...
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Visual Studio compiler not found.
    echo Please run this script from a Visual Studio Developer Command Prompt.
    echo You can find this in Start Menu -^> Visual Studio -^> Developer Command Prompt
    pause
    exit /b 1
)
echo Visual Studio compiler found.

REM Set compiler flags
echo Setting compiler flags...
set CFLAGS=/EHsc /std:c++14 /DPLATFORM_WINDOWS

REM Create output directory
echo Creating output directories...
if not exist bin\windows (
    echo Creating bin\windows directory...
    mkdir bin\windows
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: Failed to create bin\windows directory.
        pause
        exit /b 1
    )
)
echo Output directories created successfully.

REM Compile source files
echo.
echo ===================================================
echo Compiling source files...
echo ===================================================

echo Compiling Vector3.cpp...
cl %CFLAGS% /c Vector3.cpp
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile Vector3.cpp
    pause
    exit /b 1
)

echo Compiling SuperSimplePhysicsDemo_Windows.cpp...
cl %CFLAGS% /c SuperSimplePhysicsDemo_Windows.cpp
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile SuperSimplePhysicsDemo_Windows.cpp
    pause
    exit /b 1
)

REM Link object files
echo.
echo ===================================================
echo Linking object files...
echo ===================================================
echo link Vector3.obj SuperSimplePhysicsDemo_Windows.obj /OUT:bin\windows\SuperSimplePhysicsDemo.exe
link Vector3.obj SuperSimplePhysicsDemo_Windows.obj /OUT:bin\windows\SuperSimplePhysicsDemo.exe
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to link object files.
    pause
    exit /b 1
)

echo.
echo ===================================================
echo Build complete.
echo ===================================================
if exist bin\windows\SuperSimplePhysicsDemo.exe (
    echo SuperSimplePhysicsDemo.exe created successfully in bin\windows directory.
    echo File size: 
    dir bin\windows\SuperSimplePhysicsDemo.exe | findstr "SuperSimplePhysicsDemo.exe"
) else (
    echo ERROR: Build failed. SuperSimplePhysicsDemo.exe was not created.
    pause
    exit /b 1
)

echo.
echo Press any key to exit...
pause > nul
exit /b 0
