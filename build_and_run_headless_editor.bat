@echo off
setlocal enabledelayedexpansion

REM Build the headless editor
call build_headless_editor.bat

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    REM Run the headless editor
    call run_headless_editor.bat
) else (
    echo Cannot run headless editor due to build failure.
)
