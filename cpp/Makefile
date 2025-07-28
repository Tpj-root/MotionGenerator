# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Output binary
TARGET = motion_test

# Source files
SRCS = main.cpp MotionGenerator.cpp

# Object files (optional, not used here)
OBJS = $(SRCS:.cpp=.o)

# Default build rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up build artifacts
clean:
	rm -f $(TARGET) *.o motion_output.csv motion_plot.png
