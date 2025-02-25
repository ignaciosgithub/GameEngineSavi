CXX = g++
CXXFLAGS = -std=c++11 -Wall -I../
LDFLAGS = 

# Engine source files needed for tests
ENGINE_SOURCES = ../Vector3.cpp ../PhysicsSystem.cpp ../RigidBody.cpp ../GameObject.cpp ../CollisionSystem.cpp ../Time.cpp ../Scene.cpp ../EngineCondition.cpp

# Test source files
TEST_SOURCES = src/TestRunner.cpp

# All source files
SOURCES = $(TEST_SOURCES) $(ENGINE_SOURCES)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = run_tests

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
