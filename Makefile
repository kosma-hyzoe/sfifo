CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++20

SRCS_CLI = cli.cpp sfifo.cpp
SRCS_SRV = srv.cpp sfifo.cpp

OBJS_CLI = $(SRCS_CLI:.cpp=.o)
OBJS_SRV = $(SRCS_SRV:.cpp=.o)

TARGET_CLI = bin/sfifo-cli
TARGET_SRV = bin/sfifo-srv

all: prepare $(TARGET_CLI) $(TARGET_SRV)

$(TARGET_CLI): $(OBJS_CLI)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_SRV): $(OBJS_SRV)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_CLI) $(OBJS_SRV) $(TARGET_CLI) $(TARGET_SRV)
	rmdir bin

prepare:
	mkdir -p bin

.PHONY: clean preapre all

