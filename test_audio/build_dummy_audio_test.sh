#!/bin/bash

# Build dummy audio test program
echo "Building dummy audio test program..."
g++ -std=c++14 DummyAudioTest.cpp ../Audio/*.cpp \
    -I.. -DHEADLESS_ENVIRONMENT=1 -o dummy_audio_test

# Make executable
chmod +x dummy_audio_test

echo "Build complete. Run ./dummy_audio_test to test audio system."
