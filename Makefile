# Makefile for GameEngineSavi

# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g -I./ThirdParty/stb

# Define different LDFLAGS for different targets
OPENGL_LDFLAGS = -lGL -lGLU -lX11
AUDIO_LDFLAGS = -lSDL2 -lSDL2_mixer
SIMPLE_LDFLAGS =

# Define include paths
INCLUDES = -I.

# Define source files
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Define targets
TARGETS = main35engine SuperSimplePhysicsDemo LinuxPhysicsDemo

# Default target
all: $(TARGETS)

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGETS)

# Rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rules for targets
main35engine: main35engine.o Model.o Texture.o Scene.o Camera.o Vector3.o Matrix4x4.o PointLight.o GameObject.o MonoBehaviourLike.o PhysicsSystem.o CollisionSystem.o Time.o EngineTime.o EngineCondition.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENGL_LDFLAGS)

SuperSimplePhysicsDemo: SuperSimplePhysicsDemo.o Model.o Texture.o Scene.o Camera.o Vector3.o Matrix4x4.o PointLight.o GameObject.o MonoBehaviourLike.o PhysicsSystem.o CollisionSystem.o Time.o EngineTime.o EngineCondition.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENGL_LDFLAGS)

LinuxPhysicsDemo: LinuxPhysicsDemo.o Model.o Texture.o Scene.o Camera.o Vector3.o Matrix4x4.o PointLight.o GameObject.o MonoBehaviourLike.o PhysicsSystem.o CollisionSystem.o Time.o EngineTime.o EngineCondition.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENGL_LDFLAGS)

# Audio test target
AudioTest: Audio/AudioTest.o Audio/AudioSystem.o Audio/AudioSource.o Audio/AudioClip.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENGL_LDFLAGS) $(AUDIO_LDFLAGS)

# Add SDL2 and SDL2_mixer flags for audio-related targets
Audio/%.o: Audio/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I/usr/include/SDL2 -c $< -o $@

# Phony targets
.PHONY: all clean
