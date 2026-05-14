# Makefile for pmsearch
# Written by	: Khoa Nguyen
# Last updated	: 5/14/26

CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++20 -pthread

TARGET = pmsearch

# Update here if more source files are written
SRCS = main.cpp

# Default rule: what happens when you just type 'make'
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Clean rule: remove the binary to start fresh
clean:
	rm -f $(TARGET)