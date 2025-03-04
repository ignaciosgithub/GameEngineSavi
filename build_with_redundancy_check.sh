#!/bin/bash

# Build with redundancy check
echo "Building with redundancy check..."

# First, compile the debugger and redundancy detector
g++ -std=c++11 -I. -c Debugger.cpp -o bin/linux/Debugger.o
g++ -std=c++11 -I. -c RedundancyDetector.cpp -o bin/linux/RedundancyDetector.o

# Create a simple program to run the redundancy check
cat > RedundancyCheck.cpp << 'EOL'
#include "Debugger.h"
#include <iostream>

int main() {
    std::cout << "Running redundancy check..." << std::endl;
    Debugger::GetInstance().CheckForRedundancies();
    return 0;
}
EOL

# Compile and run the redundancy check
g++ -std=c++11 -I. RedundancyCheck.cpp bin/linux/Debugger.o bin/linux/RedundancyDetector.o -o bin/linux/RedundancyCheck
./bin/linux/RedundancyCheck

# Run the normal build script
./build_editor_linux.sh

# Clean up
rm RedundancyCheck.cpp
