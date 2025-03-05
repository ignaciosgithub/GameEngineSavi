@echo off
REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Create frames directory if it doesn't exist
if not exist frames mkdir frames

REM Build the simplified emergency editor
g++ -std=c++11 -o bin\windows\SimplifiedEmergencyEditor.exe ^
    simplified_emergency_editor.cpp ^
    EngineCondition.cpp ^
    -I.

REM Check if build was successful
if %ERRORLEVEL% == 0 (
    echo Simplified emergency editor build successful.
    
    REM Run the simplified emergency editor
    echo Running simplified emergency editor...
    bin\windows\SimplifiedEmergencyEditor.exe
    
    REM Check if frames were generated
    if exist frames\frame0.ppm (
        echo Frames generated successfully.
        echo You can view the frames in the frames directory.
    ) else (
        echo No frames were generated. Check the simplified emergency editor output for errors.
    )
) else (
    echo Simplified emergency editor build failed.
)
