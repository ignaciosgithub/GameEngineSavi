#!/bin/bash

# Build comprehensive audio test program
echo "Building comprehensive audio test program..."
g++ -std=c++14 ComprehensiveAudioTest.cpp ../Audio/*.cpp \
    -I.. -DHEADLESS_ENVIRONMENT=1 -o comprehensive_audio_test

# Make executable
chmod +x comprehensive_audio_test

echo "Build complete. Run ./comprehensive_audio_test to test audio system."
