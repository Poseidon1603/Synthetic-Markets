CXX = g++
CXXFLAGS = -O3 -march=native -Wall -Wextra -Wshadow -std=c++23 -g -pg -IUtils

%: %.cpp
	$(CXX) $(CXXFLAGS) $< ../Utils/utils -o $@
	./$@

clean:
	rm -f $(basename $(wildcard *.cpp))

.PHONY: clean
