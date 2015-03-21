.PHONY: all clean
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -stdlib=libc++ -Iinclude
TESTS = src/closure_manual \
        src/closure_slots \
        src/closure_wrong \
        src/net_test_leak \
        src/net_test_nonleak \
        src/pointers_and_containers \
        src/xxx
all: $(TESTS)
src/closure_manual: src/closure_manual.cpp
src/closure_slots: src/closure_slots.cpp
src/closure_wrong: src/closure_wrong.cpp
src/net_test_leak: src/net_test_leak.cpp
src/net_test_nonleak: src/net_test_nonleak.cpp
src/pointers_and_containers: src/pointers_and_containers.cpp
src/xxx: src/xxx.cpp
clean:
	rm -rf -- $(TESTS)
