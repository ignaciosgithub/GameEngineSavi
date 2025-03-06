@echo off
setlocal enabledelayedexpansion

REM Create frames directory if it doesn't exist
if not exist frames mkdir frames

REM Clean up old frames
del /Q frames\frame*.png

REM Run the frame saving editor in the background
start /B bin\windows\FrameSavingEditor.exe

REM Wait a moment for the editor to start generating frames
timeout /t 2

REM Run the frame display script
python frame_display.py

REM Note: The editor process will continue running after the frame display script exits
REM You'll need to manually close it or use taskkill in a more advanced script
