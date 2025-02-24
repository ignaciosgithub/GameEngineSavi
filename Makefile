CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -DPLATFORM_LINUX

# Source files for the Linux demo
LINUX_SOURCES = LinuxPhysicsDemo.cpp \
                PhysicsSystem.cpp \
                CollisionSystem.cpp \
                RigidBody.cpp \
                Vector3.cpp

# Object files for the Linux demo
LINUX_OBJECTS = $(LINUX_SOURCES:.cpp=.o)

# Executable for the Linux demo
LINUX_TARGET = LinuxPhysicsDemo

all: $(LINUX_TARGET)

$(LINUX_TARGET): $(LINUX_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(LINUX_OBJECTS) $(LINUX_TARGET)

.PHONY: all clean
