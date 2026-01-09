CXX = g++
CXXFLAGS =-c -O3 -march=native -Wall -Wextra -Wshadow -std=c++23 -g -pg

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(basename $(wildcard *.cpp))

.PHONY: clean
