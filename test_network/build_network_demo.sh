#!/bin/bash

echo "Building Network Demo..."

# Create bin directory if it doesn't exist
mkdir -p ../bin/linux

# Compile the network demo
g++ -std=c++14 -Wall -Wextra -I.. -I../ThirdParty -DPLATFORM_LINUX \
    NetworkDemo.cpp \
    ../Network/NetworkSystem.cpp \
    ../Network/Connection.cpp \
    ../Network/Packet.cpp \
    ../Network/NetworkDebugger.cpp \
    ../EngineCondition.cpp \
    ../ProjectSettings/ProjectSettings.cpp \
    -o ../bin/linux/NetworkDemo

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run the demo with:"
    echo "  ../bin/linux/NetworkDemo --server     # Server mode"
    echo "  ../bin/linux/NetworkDemo --client     # Client mode"
    echo "  ../bin/linux/NetworkDemo --p2p        # Peer-to-peer mode"
    echo "  ../bin/linux/NetworkDemo --debug      # Enable debugging"
else
    echo "Build failed!"
fi
