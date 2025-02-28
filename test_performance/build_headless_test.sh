#!/bin/bash
echo "Building headless performance test..."

# Create output directory if it doesn't exist
mkdir -p ../bin/linux

g++ -std=c++14 \
    HeadlessPerformanceTest.cpp \
    ../Vector3.cpp \
    ../EngineTime.cpp \
    ../Matrix4x4.cpp \
    -I.. \
    -o ../bin/linux/headless_performance_test

# Make executable
chmod +x ../bin/linux/headless_performance_test

echo "Build complete. Run ../bin/linux/headless_performance_test to test performance."
echo "You can also specify object counts as arguments: ../bin/linux/headless_performance_test 10 100 1000"
