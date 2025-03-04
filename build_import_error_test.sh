#!/bin/bash

# Build the import error test
echo "Building import error test..."

# Create bin/linux directory if it doesn't exist
mkdir -p bin/linux

# Compile the test program
g++ -std=c++11 -I. -o bin/linux/ImportErrorTest \
    ImportErrorTest.cpp \
    Debugger.cpp \
    RedundancyDetector.cpp \
    -lGL -lGLU -lglut

echo "Note: This is a simplified test that only tests the Debugger's import error handling."
echo "The full test with actual asset loading would require more dependencies."

# Make the script executable
chmod +x bin/linux/ImportErrorTest

echo "Build complete. Run with ./bin/linux/ImportErrorTest"
