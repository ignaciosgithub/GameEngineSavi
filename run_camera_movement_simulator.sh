#!/bin/bash

# Create frames directory if it doesn't exist
mkdir -p frames

# Clean up old frames
rm -f frames/frame*.png

# Generate frames with camera movement
python3 camera_movement_simulator.py

# Run the frame display script to view the frames
python3 frame_display.py
