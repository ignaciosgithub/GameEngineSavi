@echo off
echo Compiling test_platform.cpp for Windows...
g++ -std=c++14 test_platform.cpp -o test_platform_windows.exe
if %ERRORLEVEL% NEQ 0 (
    echo Windows compilation failed
    exit /b %ERRORLEVEL%
)
echo Windows compilation successful
test_platform_windows.exe
