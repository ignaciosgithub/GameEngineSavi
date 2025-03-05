#!/bin/bash

echo "Checking for headless environment..."

# Check if DISPLAY is set
if [ -z "$DISPLAY" ]; then
    echo "Warning: No display detected. This is a headless environment."
    echo "Running OpenGL applications directly may fail."
    echo ""
    echo "Options for running in headless mode:"
    echo "1. Use Xvfb (virtual framebuffer):"
    echo "   sudo apt-get install xvfb"
    echo "   Xvfb :1 -screen 0 1024x768x24 &"
    echo "   DISPLAY=:1 ./your_application"
    echo ""
    echo "2. Use the enhanced emergency editor with the headless script:"
    echo "   ./run_enhanced_emergency_editor_headless.sh"
    echo ""
    echo "3. Use X11 forwarding if connecting via SSH:"
    echo "   ssh -X user@host"
    echo ""
    echo "4. Set up a remote display:"
    echo "   export DISPLAY=remote_host:0.0"
else
    echo "Display detected: $DISPLAY"
    echo "This is not a headless environment."
    echo "OpenGL applications should run normally."
fi

# Check for OpenGL libraries
echo ""
echo "Checking for required OpenGL libraries..."
if dpkg -l | grep -q libglew-dev; then
    echo "✓ GLEW development package is installed."
else
    echo "✗ GLEW development package is not installed."
    echo "  Install with: sudo apt-get install libglew-dev"
fi

if dpkg -l | grep -q libglut3-dev; then
    echo "✓ GLUT development package is installed."
else
    echo "✗ GLUT development package is not installed."
    echo "  Install with: sudo apt-get install libglut3-dev"
fi

if dpkg -l | grep -q libgl1-mesa-dev; then
    echo "✓ OpenGL development package is installed."
else
    echo "✗ OpenGL development package is not installed."
    echo "  Install with: sudo apt-get install libgl1-mesa-dev"
fi

if dpkg -l | grep -q libglu1-mesa-dev; then
    echo "✓ GLU development package is installed."
else
    echo "✗ GLU development package is not installed."
    echo "  Install with: sudo apt-get install libglu1-mesa-dev"
fi

if dpkg -l | grep -q libx11-dev; then
    echo "✓ X11 development package is installed."
else
    echo "✗ X11 development package is not installed."
    echo "  Install with: sudo apt-get install libx11-dev"
fi
