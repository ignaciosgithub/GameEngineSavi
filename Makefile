CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -DPLATFORM_LINUX

# Source files for the simplified Linux demo
SIMPLE_SOURCES = SimplifiedPhysicsDemo.cpp \
                 PhysicsSystem.cpp \
                 Vector3.cpp

# Object files for the simplified Linux demo
SIMPLE_OBJECTS = $(SIMPLE_SOURCES:.cpp=.o)

# Executable for the simplified Linux demo
SIMPLE_TARGET = SimplifiedPhysicsDemo

all: $(SIMPLE_TARGET)

$(SIMPLE_TARGET): $(SIMPLE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SIMPLE_OBJECTS) $(SIMPLE_TARGET)

# Windows build instructions (for documentation)
windows:
	@echo "To build on Windows:"
	@echo "1. Open Visual Studio and create a new C++ project"
	@echo "2. Add all source files to the project"
	@echo "3. Set the project to use C++11 or later"
	@echo "4. Link against OpenGL libraries (opengl32.lib, glu32.lib)"
	@echo "5. Build the project"

.PHONY: all clean windows
