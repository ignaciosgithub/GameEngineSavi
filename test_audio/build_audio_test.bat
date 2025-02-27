@echo off
echo Building audio test program...

REM Ensure output directories exist
if not exist bin mkdir bin
if not exist bin\windows mkdir bin\windows

REM Build audio test program
g++ -std=c++14 AudioTest.cpp ..\Audio\*.cpp ..\Scene.cpp ..\GameObject.cpp ..\MonoBehaviourLike.cpp ^
    -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -o audio_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)

echo Build complete. Run audio_test.exe to test audio playback.
pause
