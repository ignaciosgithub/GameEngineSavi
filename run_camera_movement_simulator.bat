@echo off
setlocal enabledelayedexpansion

REM Create frames directory if it doesn't exist
if not exist frames mkdir frames

REM Clean up old frames
del /Q frames\frame*.png

REM Generate frames with camera movement
python camera_movement_simulator.py

REM Run the frame display script to view the frames
python frame_display.py
