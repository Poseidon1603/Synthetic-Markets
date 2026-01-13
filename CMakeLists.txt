cmake_minimum_required(VERSION 3.25)
project(LearningProject CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wall -Wextra -Wshadow -g -pg")

# Utils library
add_library(Utils STATIC Utils/utils.cpp Utils/utils.h)

# Function to add regimes automatically
function(add_regime REGIME_DIR)
    # Gather all cpp files
    file(GLOB_RECURSE CPP_FILES "${REGIME_DIR}/*.cpp")

    # Separate main.cpp if it exists
    set(LIBRARY_SOURCES "")
    set(EXECUTABLE_MAIN "")
    foreach(FILE ${CPP_FILES})
        get_filename_component(FILENAME ${FILE} NAME)
        if(FILENAME STREQUAL "main.cpp")
            set(EXECUTABLE_MAIN ${FILE})
        else()
            list(APPEND LIBRARY_SOURCES ${FILE})
        endif()
    endforeach()

    # Library for the regime
    get_filename_component(REGIME_NAME ${REGIME_DIR} NAME)
    add_library(${REGIME_NAME} STATIC ${LIBRARY_SOURCES})
    target_link_libraries(${REGIME_NAME} PRIVATE Utils)

    # single executable
    if(EXECUTABLE_MAIN)
        add_executable(${REGIME_NAME}Exec ${EXECUTABLE_MAIN})
        target_link_libraries(${REGIME_NAME}Exec PRIVATE ${REGIME_NAME} Utils)
    endif()
endfunction()

# Scan all subdirectories automatically
file(GLOB SUBDIRS RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/*)
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${SUBDIR} AND NOT SUBDIR STREQUAL "Utils")
        add_regime(${SUBDIR})
    endif()
endforeach()

# MainController executable
add_executable(MainController MainController/main.cpp)
# Link all regimes automatically
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${SUBDIR} AND NOT SUBDIR STREQUAL "Utils" AND NOT SUBDIR STREQUAL "MainController")
        target_link_libraries(MainController PRIVATE ${SUBDIR})
    endif()
endforeach()
target_link_libraries(MainController PRIVATE Utils)
