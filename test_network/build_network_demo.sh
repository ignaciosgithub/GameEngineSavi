#!/bin/bash
echo "Building Network Demo..."

# Create bin/linux directory if it doesn't exist
mkdir -p ../bin/linux

# Set compiler flags
CXXFLAGS="-std=c++14 -I.. -I../ThirdParty -DPLATFORM_LINUX"

# Build the network demo
g++ $CXXFLAGS \
    NetworkDemo.cpp \
    ../Network/NetworkSystem.cpp \
    ../Network/Connection.cpp \
    ../Network/Packet.cpp \
    ../Network/NetworkComponent.cpp \
    ../Network/NetworkDebugger.cpp \
    ../Vector3.cpp \
    ../Scene.cpp \
    ../EngineCondition.cpp \
    ../ProjectSettings/ProjectSettings.cpp \
    -o ../bin/linux/NetworkDemo

# Check if build was successful
if [ $? -ne 0 ]; then
    echo
    echo "Build failed!"
    exit 1
fi

echo
echo "Build successful! Network demo is available at ../bin/linux/NetworkDemo"
echo "Run with: ../bin/linux/NetworkDemo [--server|--p2p]"
