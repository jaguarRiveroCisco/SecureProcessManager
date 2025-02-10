cmake_minimum_required(VERSION 3.30)

project(DynamicControlOfChildProcesses)

set(CMAKE_CXX_STANDARD 20)

# Add -Wall flag for the C++ compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wunused-function")

# Include directories
include(cmake/include_directories.cmake)

# Include common sources
include(cmake/common_sources.cmake)

# Create a library
add_library(ProcessControllerLib STATIC ${COMMON_SOURCES})

# Main executable
add_executable(ProcessController src/ProcessController.cpp)

# Link the library to the main executable
target_link_libraries(ProcessController ProcessControllerLib)

# Include Google Test configuration
include(cmake/gtest_config.cmake)