CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I. -IThirdParty -DPLATFORM_LINUX

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
                Vector3.cpp \
                Texture.cpp \
                EngineTime.cpp

# Source files for the physics system
PHYSICS_SOURCES = CollisionSystem.cpp \
                 PhysicsSystem.cpp \
                 RigidBody.cpp

# Source files for the animation system
ANIMATION_SOURCES = Animation/KeyFrame.cpp \
                   Animation/Animation.cpp \
                   Animation/AnimationComponent.cpp \
                   Animation/AnimationLoader.cpp

# Source files for the networking system
NETWORK_SOURCES = Network/NetworkSystem.cpp \
                 Network/Connection.cpp \
                 Network/Packet.cpp \
                 Network/NetworkComponent.cpp \
                 Network/NetworkDebugger.cpp

# Source files for the super simple Linux demo
SUPER_SIMPLE_SOURCES = SuperSimplePhysicsDemo.cpp \
                       Vector3.cpp

# Source files for the Linux physics demo
LINUX_PHYSICS_DEMO_SOURCES = LinuxPhysicsDemo.cpp \
                            Vector3.cpp \
                            CollisionSystem.cpp \
                            PhysicsSystem.cpp \
                            RigidBody.cpp \
                            EngineCondition.cpp \
                            ProjectSettings/ProjectSettings.cpp \
                            ProjectSettings/ProjectManager.cpp

# Source files for the network demo
NETWORK_DEMO_SOURCES = test_network/NetworkDemo.cpp \
                      Network/NetworkSystem.cpp \
                      Network/Connection.cpp \
                      Network/Packet.cpp \
                      Network/NetworkComponent.cpp \
                      Network/NetworkDebugger.cpp \
                      Vector3.cpp \
                      Scene.cpp \
                      EngineCondition.cpp \
                      ProjectSettings/ProjectSettings.cpp

# Source files for the GUI system
GUI_SOURCES = GUI/GUI.cpp \
             GUI/SettingsPanel.cpp \
             GUI/TextField.cpp

# Source files for the editor
EDITOR_SOURCES = Editor/Editor.cpp \
                Editor/HierarchyPanel.cpp \
                Editor/SceneViewPanel.cpp \
                Editor/InspectorPanel.cpp \
                Editor/ProjectPanel.cpp \
                Editor/EditorMain.cpp \
                Editor/Vector3Field.cpp \
                Editor/AnimationPanel.cpp

# Source files for the animation test
ANIMATION_TEST_SOURCES = test_animations/AnimationTest.cpp \
                        $(ANIMATION_SOURCES) \
                        Vector3.cpp \
                        Model.cpp

# Source files for the main engine demo
MAIN_ENGINE_SOURCES = main35engine.cpp \
                     $(ENGINE_SOURCES)

# Object files
ENGINE_OBJECTS = $(ENGINE_SOURCES:.cpp=.o)
PHYSICS_OBJECTS = $(PHYSICS_SOURCES:.cpp=.o)
ANIMATION_OBJECTS = $(ANIMATION_SOURCES:.cpp=.o)
NETWORK_OBJECTS = $(NETWORK_SOURCES:.cpp=.o)
SUPER_SIMPLE_OBJECTS = $(SUPER_SIMPLE_SOURCES:.cpp=.o)
LINUX_PHYSICS_DEMO_OBJECTS = $(LINUX_PHYSICS_DEMO_SOURCES:.cpp=.o)
GUI_OBJECTS = $(GUI_SOURCES:.cpp=.o)
EDITOR_OBJECTS = $(EDITOR_SOURCES:.cpp=.o)
ANIMATION_TEST_OBJECTS = $(ANIMATION_TEST_SOURCES:.cpp=.o)
MAIN_ENGINE_OBJECTS = $(MAIN_ENGINE_SOURCES:.cpp=.o) $(GUI_OBJECTS)
NETWORK_DEMO_OBJECTS = $(NETWORK_DEMO_SOURCES:.cpp=.o)

# Executables
SUPER_SIMPLE_TARGET = bin/linux/SuperSimplePhysicsDemo
LINUX_PHYSICS_DEMO_TARGET = bin/linux/LinuxPhysicsDemo
MAIN_ENGINE_TARGET = bin/linux/MainEngine
EDITOR_TARGET = bin/linux/Editor
ANIMATION_TEST_TARGET = bin/linux/AnimationTest
NETWORK_DEMO_TARGET = bin/linux/NetworkDemo

# Default target
all: directories $(SUPER_SIMPLE_TARGET) $(LINUX_PHYSICS_DEMO_TARGET) $(EDITOR_TARGET) $(ANIMATION_TEST_TARGET) $(NETWORK_DEMO_TARGET)

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
	
# Build the editor
$(EDITOR_TARGET): $(EDITOR_OBJECTS) $(ENGINE_OBJECTS) $(PHYSICS_OBJECTS) $(GUI_OBJECTS) $(ANIMATION_OBJECTS) ProjectSettings/ProjectSettings.o ProjectSettings/ProjectManager.o EngineCondition.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENGL_LDFLAGS)

# Build the animation test
$(ANIMATION_TEST_TARGET): $(ANIMATION_TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SIMPLE_LDFLAGS)
	
# Build the network demo
$(NETWORK_DEMO_TARGET): $(NETWORK_DEMO_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SIMPLE_LDFLAGS)

# Build the engine library
libengine.a: $(ENGINE_OBJECTS) $(PHYSICS_OBJECTS) $(ANIMATION_OBJECTS)
	ar rcs $@ $^

# Generic rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(ENGINE_OBJECTS) $(PHYSICS_OBJECTS) $(ANIMATION_OBJECTS) $(SUPER_SIMPLE_OBJECTS) $(LINUX_PHYSICS_DEMO_OBJECTS) $(MAIN_ENGINE_OBJECTS) $(EDITOR_OBJECTS) $(ANIMATION_TEST_OBJECTS)
	rm -f $(SUPER_SIMPLE_TARGET) $(LINUX_PHYSICS_DEMO_TARGET) $(MAIN_ENGINE_TARGET) $(EDITOR_TARGET) $(ANIMATION_TEST_TARGET) libengine.a

# Windows build instructions (for documentation)
windows:
	@echo "To build on Windows:"
	@echo "1. Open Visual Studio"
	@echo "2. Create a new project and add all source files"
	@echo "3. Set the project to use C++14 or later"
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
	@echo "  all                  - Build the super simple physics demo, Linux physics demo, editor, and animation test"
	@echo "  $(SUPER_SIMPLE_TARGET)   - Build the super simple physics demo"
	@echo "  $(LINUX_PHYSICS_DEMO_TARGET)   - Build the Linux physics demo"
	@echo "  $(MAIN_ENGINE_TARGET)   - Build the main engine demo"
	@echo "  $(EDITOR_TARGET)   - Build the editor"
	@echo "  $(ANIMATION_TEST_TARGET)   - Build the animation test"
	@echo "  libengine.a         - Build the engine as a static library"
	@echo "  clean               - Remove all built files"
	@echo "  windows             - Show Windows build instructions"
	@echo "  help                - Show this help message"

.PHONY: all directories clean windows help

# Project management system
PROJECT_SOURCES = ProjectSettings/ProjectSettings.cpp ProjectSettings/ProjectManager.cpp
PROJECT_OBJECTS = $(PROJECT_SOURCES:.cpp=.o)

# Add project management objects to the OBJECTS variable
OBJECTS = $(PROJECT_OBJECTS)

# Debugger system
DEBUGGER_SOURCES = Debugger.cpp
DEBUGGER_OBJECTS = $(DEBUGGER_SOURCES:.cpp=.o)

# Add debugger systems to all targets
OBJECTS += $(DEBUGGER_OBJECTS)
