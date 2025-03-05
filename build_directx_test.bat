@echo off
REM build_directx_test.bat

echo Testing DirectX implementation...

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g
set INCLUDES=-I. -IThirdParty/DirectX/include

REM Create output directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Compile DirectX test
echo Compiling DirectXGraphicsAPI...
g++ %CFLAGS% %INCLUDES% -c Graphics\Core\DirectXGraphicsAPI.cpp -o bin\windows\DirectXGraphicsAPI.o -DPLATFORM_WINDOWS

if %ERRORLEVEL% EQU 0 (
    echo DirectX implementation compiled successfully.
) else (
    echo Error: DirectX implementation compilation failed.
)
