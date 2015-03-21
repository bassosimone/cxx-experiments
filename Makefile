.PHONY: all clean
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -stdlib=libc++ -Iinclude
TESTS = src/callbacks \
        src/net_test_leak \
        src/net_test_nonleak \
        src/pointers_and_containers \
        src/xxx
all: $(TESTS)
src/callbacks: src/callbacks.cpp
src/net_test_leak: src/net_test_leak.cpp
src/net_test_nonleak: src/net_test_nonleak.cpp
src/pointers_and_containers: src/pointers_and_containers.cpp
src/xxx: src/xxx.cpp
clean:
	rm -rf -- $(TESTS)
