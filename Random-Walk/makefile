CXX = g++
CXXFLAGS = -O3 -march=native -Wall -Wextra -Wshadow -std=c++20 -g

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	./$@

clean:
	rm -f $(basename $(wildcard *.cpp))

.PHONY: clean
