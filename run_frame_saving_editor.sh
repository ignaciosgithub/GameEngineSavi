#!/bin/bash

# Create frames directory if it doesn't exist
mkdir -p frames

# Clean up old frames
rm -f frames/frame*.png

# Run the frame saving editor in the background
./bin/linux/FrameSavingEditor &
EDITOR_PID=$!

# Wait a moment for the editor to start generating frames
sleep 2

# Run the frame display script
python3 frame_display.py

# When the frame display script exits, kill the editor
kill $EDITOR_PID
