#!/bin/bash

# Build standalone test program
echo "Building standalone test program..."
g++ -std=c++14 StandaloneTest.cpp -o standalone_test

# Make executable
chmod +x standalone_test

echo "Build complete. Run ./standalone_test to test components."
