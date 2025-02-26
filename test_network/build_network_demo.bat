@echo off
echo Building Network Demo...

:: Create bin directory if it doesn't exist
if not exist ..\bin\windows mkdir ..\bin\windows

:: Compile the network demo
g++ -std=c++14 -Wall -Wextra -I.. -I../ThirdParty -DPLATFORM_WINDOWS ^
    NetworkDemo.cpp ^
    ../Network/NetworkSystem.cpp ^
    ../Network/Connection.cpp ^
    ../Network/Packet.cpp ^
    ../Network/NetworkDebugger.cpp ^
    ../EngineCondition.cpp ^
    ../ProjectSettings/ProjectSettings.cpp ^
    -o ../bin/windows/NetworkDemo.exe -lws2_32

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Run the demo with:
    echo   ..\bin\windows\NetworkDemo.exe --server     # Server mode
    echo   ..\bin\windows\NetworkDemo.exe --client     # Client mode
    echo   ..\bin\windows\NetworkDemo.exe --p2p        # Peer-to-peer mode
    echo   ..\bin\windows\NetworkDemo.exe --debug      # Enable debugging
) else (
    echo Build failed!
)

pause
