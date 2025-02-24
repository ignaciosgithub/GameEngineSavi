@echo off
echo ===================================================
echo Running Physics Demo...
echo ===================================================

REM Check if the executable exists
echo Checking for physics demo executables...

if exist bin\windows\PhysicsDemo.exe (
    echo Found PhysicsDemo.exe
    echo File size: 
    dir bin\windows\PhysicsDemo.exe | findstr "PhysicsDemo.exe"
    echo.
    echo ===================================================
    echo Running PhysicsDemo.exe...
    echo ===================================================
    bin\windows\PhysicsDemo.exe
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: PhysicsDemo.exe exited with error code %ERRORLEVEL%
    ) else (
        echo PhysicsDemo.exe completed successfully.
    )
) else if exist bin\windows\SuperSimplePhysicsDemo.exe (
    echo Found SuperSimplePhysicsDemo.exe
    echo File size: 
    dir bin\windows\SuperSimplePhysicsDemo.exe | findstr "SuperSimplePhysicsDemo.exe"
    echo.
    echo ===================================================
    echo Running SuperSimplePhysicsDemo.exe...
    echo ===================================================
    bin\windows\SuperSimplePhysicsDemo.exe
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: SuperSimplePhysicsDemo.exe exited with error code %ERRORLEVEL%
    ) else (
        echo SuperSimplePhysicsDemo.exe completed successfully.
    )
) else (
    echo ERROR: No physics demo executable found.
    echo Please run build_windows.bat or build_windows_simple.bat first.
    echo.
    echo Current directory contents:
    dir
    echo.
    echo bin\windows directory contents:
    if exist bin\windows (
        dir bin\windows
    ) else (
        echo bin\windows directory does not exist.
    )
)

echo.
echo Press any key to exit...
pause > nul
