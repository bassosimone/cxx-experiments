.PHONY: all clean
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -stdlib=libc++ -Iinclude
all: src/callbacks
src/callbacks: src/callbacks.cpp
clean:
	rm -rf -- src/callbacks
