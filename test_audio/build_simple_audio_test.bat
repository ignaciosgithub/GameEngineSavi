@echo off
echo Building simple audio test program...

REM Build simple audio test program
g++ -std=c++14 SimpleAudioTest.cpp ..\Audio\*.cpp ^
    -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -o simple_audio_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run simple_audio_test.exe to test audio playback.
pause
