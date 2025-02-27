#!/bin/bash

# Build audio test program
echo "Building audio test program..."
g++ -std=c++14 AudioTest.cpp ../Audio/*.cpp ../Scene.cpp ../GameObject.cpp ../MonoBehaviourLike.cpp \
    -I.. -I/usr/include/SDL2 -lSDL2 -lSDL2_mixer -o audio_test

# Make executable
chmod +x audio_test

echo "Build complete. Run ./audio_test to test audio playback."
