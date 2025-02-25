CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I.. -I../ThirdParty

# Source files
SOURCES = StandaloneDebuggerTest.cpp \
          ../Debugger.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable
TARGET = StandaloneDebuggerTest

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
