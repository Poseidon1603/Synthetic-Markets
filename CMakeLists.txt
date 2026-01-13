cmake_minimum_required(VERSION 3.20)
project(LearningProject CXX)

# Set our cpp version
set(CMAKE_CXX_STANDARD 23)
# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wall -Wextra -Wshadow -g -pg")

# Add Utils include path
include_directories(${PROJECT_SOURCE_DIR}/Utils)

# Build the shared library for Utils
add_library(Utils STATIC Utils/utils.cpp Utils/utils.h)

# Add your master executable
add_executable(MainController main.cpp)
target_link_libraries(MainController PRIVATE Utils)

# Add subdirectories for each regime
add_subdirectory(RandomWalk)
add_subdirectory(MeanReverting)
# add_subdirectory(OtherRegime)

