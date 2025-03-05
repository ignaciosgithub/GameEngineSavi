#!/bin/bash

# Create bin directory if it doesn't exist
mkdir -p bin/linux

# Create frames directory if it doesn't exist
mkdir -p frames

# Build the simplified emergency editor
g++ -std=c++11 -o bin/linux/SimplifiedEmergencyEditor \
    simplified_emergency_editor.cpp \
    EngineCondition.cpp \
    -I.

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Simplified emergency editor build successful."
    chmod +x bin/linux/SimplifiedEmergencyEditor
    
    # Run the simplified emergency editor
    echo "Running simplified emergency editor..."
    ./bin/linux/SimplifiedEmergencyEditor
    
    # Check if frames were generated
    if [ -d "frames" ] && [ "$(ls -A frames)" ]; then
        echo "Frames generated successfully."
        echo "You can view the frames in the frames directory."
    else
        echo "No frames were generated. Check the simplified emergency editor output for errors."
    fi
else
    echo "Simplified emergency editor build failed."
fi
