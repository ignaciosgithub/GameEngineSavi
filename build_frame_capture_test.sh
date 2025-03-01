#!/bin/bash
echo "Building frame capture test..."

# Create output directory if it doesn't exist
mkdir -p bin/linux

# Compile the test program
g++ -std=c++14 -Wall -Wextra -g -I./ThirdParty/stb \
    test_frame_capture.cpp \
    FrameCapture.cpp \
    -o bin/linux/test_frame_capture \
    -lGL -lGLU -lglut -lX11

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run with: ./bin/linux/test_frame_capture"
else
    echo "Build failed!"
fi
