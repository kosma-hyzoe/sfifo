# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++20

# Source files
SRCS_CLI = cli.cpp common.cpp
SRCS_SRV = srv.cpp common.cpp

# Object files
OBJS_CLI = $(SRCS_CLI:.cpp=.o)
OBJS_SRV = $(SRCS_SRV:.cpp=.o)

# Executable names
TARGET_CLI = bin/cli
TARGET_SRV = bin/srv

# Default target
all: $(TARGET_CLI) $(TARGET_SRV)

# Build cli executable
$(TARGET_CLI): $(OBJS_CLI)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build srv executable
$(TARGET_SRV): $(OBJS_SRV)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS_CLI) $(OBJS_SRV) $(TARGET_CLI) $(TARGET_SRV)

# Create bin directory if it doesn't exist
prepare:
	mkdir -p bin

# Phony targets
.PHONY: all clean prepare

