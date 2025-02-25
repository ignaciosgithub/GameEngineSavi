#!/bin/bash
echo "Building and Running Animation Collision Test..."
echo

# Create bin/linux directory if it doesn't exist
mkdir -p bin/linux

# Set compiler flags
CXXFLAGS="-std=c++14 -I. -IThirdParty -DPLATFORM_LINUX"

# Build the animation collision test
g++ $CXXFLAGS \
    test_animations/AnimationCollisionTest.cpp \
    Animation/KeyFrame.cpp \
    Animation/Animation.cpp \
    Animation/AnimationComponent.cpp \
    Animation/AnimationLoader.cpp \
    Model.cpp \
    Vector3.cpp \
    CollisionSystem.cpp \
    RigidBody.cpp \
    GameObject.cpp \
    Scene.cpp \
    PhysicsSystem.cpp \
    -o bin/linux/AnimationCollisionTest

# Check if build was successful
if [ $? -ne 0 ]; then
    echo
    echo "Build failed!"
    exit 1
fi

# Run the animation collision test
bin/linux/AnimationCollisionTest
