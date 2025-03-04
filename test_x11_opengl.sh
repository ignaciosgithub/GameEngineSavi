#!/bin/bash

echo "Testing X11 forwarding with OpenGL..."

# Check if X11 forwarding is working
echo "DISPLAY=$DISPLAY"
if [ -z "$DISPLAY" ]; then
    echo "Error: DISPLAY environment variable not set"
    echo "X11 forwarding is not configured correctly"
    exit 1
fi

# Check if xeyes works (basic X11 test)
echo "Testing basic X11 with xeyes..."
which xeyes > /dev/null
if [ $? -ne 0 ]; then
    echo "xeyes not found, installing x11-apps..."
    sudo apt-get update
    sudo apt-get install -y x11-apps
fi

# Run xeyes in background
xeyes &
XEYES_PID=$!
sleep 2

# Check if xeyes is running
if ps -p $XEYES_PID > /dev/null; then
    echo "xeyes is running - basic X11 forwarding works"
    kill $XEYES_PID
else
    echo "Error: xeyes failed to run"
    echo "Basic X11 forwarding is not working"
    exit 1
fi

# Check OpenGL capabilities
echo "Testing OpenGL capabilities..."
glxinfo | grep -E "OpenGL vendor|OpenGL renderer|OpenGL version|direct rendering"

# Run glxgears as an OpenGL test
echo "Testing OpenGL with glxgears..."
which glxgears > /dev/null
if [ $? -ne 0 ]; then
    echo "glxgears not found, installing mesa-utils..."
    sudo apt-get update
    sudo apt-get install -y mesa-utils
fi

# Run glxgears with frame counting for 5 seconds
echo "Running glxgears for 5 seconds..."
timeout 5s glxgears 2>&1 | grep frames

echo "X11 and OpenGL test complete"
