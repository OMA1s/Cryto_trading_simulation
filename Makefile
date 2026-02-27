# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -Iheader

# Name of the final executable (will be created in current directory)
TARGET = merklerex.exe

# Find all .cpp files inside cpp/ folder
SOURCES = $(wildcard cpp/*.cpp)

# Object files will be created in current directory (or you can change to obj/)
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link all object files → executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile each .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean