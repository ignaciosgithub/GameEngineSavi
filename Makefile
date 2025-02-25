CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -IThirdParty -DPLATFORM_LINUX

# Define different LDFLAGS for different targets
# Full OpenGL flags for graphics-related targets
OPENGL_LDFLAGS = -lGL -lGLU -lX11

# No OpenGL flags for simple demos that don't need graphics
SIMPLE_LDFLAGS =

# Source files for the engine
ENGINE_SOURCES = Camera.cpp \
                Face.cpp \
                GameObject.cpp \
                Matrix4x4.cpp \
                Model.cpp \
                MonoBehaviourLike.cpp \
                PointLight.cpp \
                Prefab.cpp \
                Pyramid.cpp \
                Raycast.cpp \
                Scene.cpp \
                Time.cpp \
                Triangle.cpp \
                Vector3.cpp

# Source files for the physics system
PHYSICS_SOURCES = CollisionSystem.cpp \
                 PhysicsSystem.cpp \
                 RigidBody.cpp

# Source files for the super simple Linux demo
SUPER_SIMPLE_SOURCES = SuperSimplePhysicsDemo.cpp \
                       Vector3.cpp

# Source files for the Linux physics demo
LINUX_PHYSICS_DEMO_SOURCES = LinuxPhysicsDemo.cpp \
                            Vector3.cpp \
                            CollisionSystem.cpp \
                            PhysicsSystem.cpp \
                            RigidBody.cpp

# Source files for the GUI system
GUI_SOURCES = GUI/GUI.cpp

# Source files for the main engine demo
MAIN_ENGINE_SOURCES = main35engine.cpp \
                     $(ENGINE_SOURCES)

# Object files
ENGINE_OBJECTS = $(ENGINE_SOURCES:.cpp=.o)
PHYSICS_OBJECTS = $(PHYSICS_SOURCES:.cpp=.o)
SUPER_SIMPLE_OBJECTS = $(SUPER_SIMPLE_SOURCES:.cpp=.o)
LINUX_PHYSICS_DEMO_OBJECTS = $(LINUX_PHYSICS_DEMO_SOURCES:.cpp=.o)
GUI_OBJECTS = $(GUI_SOURCES:.cpp=.o)
MAIN_ENGINE_OBJECTS = $(MAIN_ENGINE_SOURCES:.cpp=.o) $(GUI_OBJECTS)

# Executables
SUPER_SIMPLE_TARGET = bin/linux/SuperSimplePhysicsDemo
LINUX_PHYSICS_DEMO_TARGET = bin/linux/LinuxPhysicsDemo
MAIN_ENGINE_TARGET = bin/linux/MainEngine

# Default target
all: directories $(SUPER_SIMPLE_TARGET) $(LINUX_PHYSICS_DEMO_TARGET)

# Create necessary directories
directories:
	mkdir -p bin/linux

# Build the super simple physics demo
$(SUPER_SIMPLE_TARGET): $(SUPER_SIMPLE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SIMPLE_LDFLAGS)

# Build the Linux physics demo
$(LINUX_PHYSICS_DEMO_TARGET): $(LINUX_PHYSICS_DEMO_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SIMPLE_LDFLAGS)

# Build the main engine demo
$(MAIN_ENGINE_TARGET): $(MAIN_ENGINE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENGL_LDFLAGS)

# Build the engine library
libengine.a: $(ENGINE_OBJECTS) $(PHYSICS_OBJECTS)
	ar rcs $@ $^

# Generic rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(ENGINE_OBJECTS) $(PHYSICS_OBJECTS) $(SUPER_SIMPLE_OBJECTS) $(LINUX_PHYSICS_DEMO_OBJECTS) $(MAIN_ENGINE_OBJECTS)
	rm -f $(SUPER_SIMPLE_TARGET) $(LINUX_PHYSICS_DEMO_TARGET) $(MAIN_ENGINE_TARGET) libengine.a

# Windows build instructions (for documentation)
windows:
	@echo "To build on Windows:"
	@echo "1. Open Visual Studio and create a new C++ project"
	@echo "2. Add all source files to the project"
	@echo "3. Set the project to use C++11 or later"
	@echo "4. Link against OpenGL libraries (opengl32.lib, glu32.lib)"
	@echo "5. Build the project"
	@echo ""
	@echo "Alternatively, you can use MinGW or Cygwin with GCC on Windows:"
	@echo "1. Install MinGW or Cygwin with GCC"
	@echo "2. Open a terminal and navigate to the project directory"
	@echo "3. Run 'mingw32-make -f Makefile.mingw' or 'make -f Makefile.mingw'"

# Help target
help:
	@echo "Available targets:"
	@echo "  all                  - Build the super simple physics demo and Linux physics demo"
	@echo "  $(SUPER_SIMPLE_TARGET)   - Build the super simple physics demo"
	@echo "  $(LINUX_PHYSICS_DEMO_TARGET)   - Build the Linux physics demo"
	@echo "  $(MAIN_ENGINE_TARGET)   - Build the main engine demo"
	@echo "  libengine.a         - Build the engine as a static library"
	@echo "  clean               - Remove all built files"
	@echo "  windows             - Show Windows build instructions"
	@echo "  help                - Show this help message"

.PHONY: all directories clean windows help

# Project management system
PROJECT_SOURCES = ProjectSettings/ProjectSettings.cpp ProjectSettings/ProjectManager.cpp
PROJECT_OBJECTS = $(PROJECT_SOURCES:.cpp=.o)

# Add project management system to all targets
OBJECTS += $(PROJECT_OBJECTS)
