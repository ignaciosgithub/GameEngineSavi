@echo off
echo Building headless performance test...

REM Create output directory if it doesn't exist
if not exist ..\bin\windows mkdir ..\bin\windows

g++ -std=c++14 ^
    HeadlessPerformanceTest.cpp ^
    ..\Vector3.cpp ^
    ..\EngineTime.cpp ^
    ..\Matrix4x4.cpp ^
    -I.. ^
    -o ..\bin\windows\headless_performance_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run ..\bin\windows\headless_performance_test.exe to test performance.
echo You can also specify object counts as arguments: ..\bin\windows\headless_performance_test.exe 10 100 1000
echo.
echo Press any key to run the performance test...
pause > nul
..\bin\windows\headless_performance_test.exe 10 100 1000
