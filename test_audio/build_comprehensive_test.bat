@echo off
echo Building comprehensive audio test program...

REM Build comprehensive audio test program
g++ -std=c++14 ComprehensiveAudioTest.cpp ..\Audio\*.cpp ^
    -I.. -DHEADLESS_ENVIRONMENT=1 -o comprehensive_audio_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run comprehensive_audio_test.exe to test audio system.
pause
