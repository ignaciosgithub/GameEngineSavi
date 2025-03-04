#!/bin/bash
echo "Checking VirtualBox Guest Additions..."
lsmod | grep vbox

echo -e "\nChecking OpenGL information..."
glxinfo | grep -E "OpenGL vendor|OpenGL renderer|OpenGL version|direct rendering"

echo -e "\nChecking for software rendering environment variables..."
env | grep -E "LIBGL|GLX"

echo -e "\nChecking if 3D acceleration is working..."
glxinfo | grep -i "direct rendering"

echo -e "\nChecking for X11 display configuration..."
echo "DISPLAY=$DISPLAY"
xdpyinfo | grep -E "^screen|dimensions"
