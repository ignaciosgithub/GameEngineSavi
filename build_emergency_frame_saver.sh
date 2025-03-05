#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Create frames directory if it doesn't exist
mkdir -p frames

# Build the emergency frame saver
g++ -std=c++11 -o bin/linux/emergency_frame_saver \
    emergency_frame_saver.cpp \
    -I. \
    -IThirdParty/stb

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Emergency frame saver build successful."
    chmod +x bin/linux/emergency_frame_saver
    
    # Run the emergency frame saver to generate frames
    echo "Running emergency frame saver to generate frames..."
    ./bin/linux/emergency_frame_saver
    
    # Check if frame4.png exists
    if [ -f "frames/frame4.png" ]; then
        echo "Frame 4 generated successfully."
        echo "You can view frame4.png in the frames directory."
    else
        echo "Frame 4 was not generated. Check the emergency frame saver output for errors."
    fi
else
    echo "Emergency frame saver build failed."
fi
