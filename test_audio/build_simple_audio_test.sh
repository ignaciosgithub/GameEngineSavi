#!/bin/bash

# Build simple audio test program
echo "Building simple audio test program..."
g++ -std=c++14 SimpleAudioTest.cpp ../Audio/*.cpp \
    -I.. -I/usr/include/SDL2 -lSDL2 -lSDL2_mixer -o simple_audio_test

# Make executable
chmod +x simple_audio_test

echo "Build complete. Run ./simple_audio_test to test audio playback."
