#!/bin/bash

# Create frames directory if it doesn't exist
mkdir -p frames

# Check if Xvfb is installed
if ! command -v Xvfb &> /dev/null; then
    echo "Xvfb not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y xvfb
fi

# Check if the enhanced emergency editor exists
if [ ! -f "bin/linux/EnhancedEmergencyEditor" ]; then
    echo "Enhanced emergency editor not found. Building..."
    ./build_enhanced_emergency_editor.sh
fi

# Run the enhanced emergency editor in a virtual X server
echo "Running enhanced emergency editor in headless mode..."
Xvfb :1 -screen 0 1024x768x24 &
XVFB_PID=$!

# Wait for Xvfb to start
sleep 2

# Run the enhanced emergency editor
DISPLAY=:1 ./bin/linux/EnhancedEmergencyEditor

# Kill Xvfb
kill $XVFB_PID

# Check if frames were generated
if [ -d "frames" ] && [ "$(ls -A frames)" ]; then
    echo "Frames generated successfully."
    echo "You can view the frames using the pygame frame reader."
else
    echo "No frames were generated. Check the enhanced emergency editor output for errors."
fi
