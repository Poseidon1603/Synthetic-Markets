# Build RandomWalk library
add_library(RandomWalk STATIC randomwalk.cpp randomwalk.h)
target_link_libraries(RandomWalk PRIVATE Utils)

# standalone executable
add_executable(RandomWalkExec main.cpp)
target_link_libraries(RandomWalkExec PRIVATE RandomWalk Utils)
