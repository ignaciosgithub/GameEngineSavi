@echo off
setlocal enabledelayedexpansion

REM Create frames directory if it doesn't exist
if not exist frames mkdir frames

REM Clean up old frames (both PNG and PPM)
del /Q frames\frame*.png
del /Q frames\frame*.ppm
del /Q frames\hotkey_state.txt
del /Q frames\panel_info.txt
del /Q frames\status.txt

REM Run the headless editor in the background
start /B bin\windows\HeadlessEditor.exe

REM Wait for initialization to complete (max 10 seconds)
echo Waiting for headless editor to initialize...
set initialized=0
for /L %%i in (1,1,20) do (
    if exist frames\status.txt (
        echo Headless editor initialized successfully
        set initialized=1
        goto :continue
    )
    timeout /t 1 /nobreak >nul
)
:continue

REM Run the frame display script
python frame_display.py

REM Note: The editor process will continue running after the frame display script exits
REM You'll need to manually close it or use taskkill in a more advanced script
