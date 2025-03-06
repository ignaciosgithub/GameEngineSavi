#!/bin/bash

# Build the headless editor
./build_headless_editor.sh

# Check if build was successful
if [ $? -eq 0 ]; then
    # Run the headless editor
    ./run_headless_editor.sh
else
    echo "Cannot run headless editor due to build failure."
fi
