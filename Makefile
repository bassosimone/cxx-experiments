.PHONY: all clean
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -stdlib=libc++ -Iinclude
TESTS = src/callbacks \
        src/pointers_and_containers
all: $(TESTS)
src/callbacks: src/callbacks.cpp
src/pointers_and_containers: src/pointers_and_containers.cpp
clean:
	rm -rf -- $(TESTS)
