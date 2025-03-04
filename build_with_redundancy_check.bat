@echo off
echo Building with redundancy check...

REM First, compile the debugger and redundancy detector
g++ -std=c++11 -I. -c Debugger.cpp -o bin\windows\Debugger.o
g++ -std=c++11 -I. -c RedundancyDetector.cpp -o bin\windows\RedundancyDetector.o

REM Create a simple program to run the redundancy check
echo #include "Debugger.h" > RedundancyCheck.cpp
echo #include ^<iostream^> >> RedundancyCheck.cpp
echo. >> RedundancyCheck.cpp
echo int main() { >> RedundancyCheck.cpp
echo     std::cout ^<^< "Running redundancy check..." ^<^< std::endl; >> RedundancyCheck.cpp
echo     Debugger::GetInstance().CheckForRedundancies(); >> RedundancyCheck.cpp
echo     return 0; >> RedundancyCheck.cpp
echo } >> RedundancyCheck.cpp

REM Compile and run the redundancy check
g++ -std=c++11 -I. RedundancyCheck.cpp bin\windows\Debugger.o bin\windows\RedundancyDetector.o -o bin\windows\RedundancyCheck.exe
bin\windows\RedundancyCheck.exe

REM Run the normal build script
call build_all_windows_complete.bat

REM Clean up
del RedundancyCheck.cpp
