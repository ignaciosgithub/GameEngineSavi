@echo off
echo Running Physics Demo...

REM Check if the executable exists
if exist bin\windows\PhysicsDemo.exe (
  bin\windows\PhysicsDemo.exe
) else if exist bin\windows\SuperSimplePhysicsDemo.exe (
  bin\windows\SuperSimplePhysicsDemo.exe
) else (
  echo Error: No physics demo executable found.
  echo Please run build_windows.bat or build_windows_simple.bat first.
  exit /b 1
)
