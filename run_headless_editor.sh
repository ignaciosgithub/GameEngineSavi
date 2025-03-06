#!/bin/bash

# Create frames directory if it doesn't exist
mkdir -p frames

# Clean up old frames (both PNG and PPM)
rm -f frames/frame*.png
rm -f frames/frame*.ppm
rm -f frames/hotkey_state.txt
rm -f frames/panel_info.txt
rm -f frames/status.txt

# Run the headless editor in the background
./bin/linux/HeadlessEditor &
EDITOR_PID=$!

# Wait for initialization to complete (max 10 seconds)
echo "Waiting for headless editor to initialize..."
for i in {1..20}; do
    if [ -f frames/status.txt ]; then
        echo "Headless editor initialized successfully"
        break
    fi
    sleep 0.5
done

# Run the frame display script
python3 frame_display.py

# When the frame display script exits, kill the headless editor
kill $EDITOR_PID
