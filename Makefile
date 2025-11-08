CXX = g++-13

CXXFLAGS = -Wall -Wextra -std=c++20

SRCS_CLI = cli.cpp sfifo.cpp
SRCS_SRV = srv.cpp sfifo.cpp

OBJS_CLI = $(SRCS_CLI:.cpp=.o)
OBJS_SRV = $(SRCS_SRV:.cpp=.o)

TARGET_CLI = bin/sfifo-cli
TARGET_SRV = bin/sfifo-srv

all: $(TARGET_CLI) $(TARGET_SRV)

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
.PHONY: clean preapre all

