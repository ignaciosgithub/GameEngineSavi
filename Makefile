CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -DPLATFORM_LINUX

# Source files for the super simple Linux demo
SUPER_SIMPLE_SOURCES = SuperSimplePhysicsDemo.cpp \
                       Vector3.cpp

# Object files for the super simple Linux demo
SUPER_SIMPLE_OBJECTS = $(SUPER_SIMPLE_SOURCES:.cpp=.o)

# Executable for the super simple Linux demo
SUPER_SIMPLE_TARGET = SuperSimplePhysicsDemo

all: $(SUPER_SIMPLE_TARGET)

$(SUPER_SIMPLE_TARGET): $(SUPER_SIMPLE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SUPER_SIMPLE_OBJECTS) $(SUPER_SIMPLE_TARGET)

# Windows build instructions (for documentation)
windows:
	@echo "To build on Windows:"
	@echo "1. Open Visual Studio and create a new C++ project"
	@echo "2. Add all source files to the project"
	@echo "3. Set the project to use C++11 or later"
	@echo "4. Link against OpenGL libraries (opengl32.lib, glu32.lib)"
	@echo "5. Build the project"

.PHONY: all clean windows
