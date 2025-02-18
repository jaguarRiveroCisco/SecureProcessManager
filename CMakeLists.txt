cmake_minimum_required(VERSION 3.30)

# Detect platform before setting compilers
if(APPLE)
    set(IS_MACOS TRUE)
else()
    set(IS_MACOS FALSE)
endif()

# Include the set_compilers.cmake file
include(cmake/set_compilers.cmake)

# Call the function
set_compilers()

project(DynamicControlOfChildProcesses)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add -Wall flag for the C++ compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wunused-function -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# Add -g flag for the debug build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Print the compilers being used
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

# Include directories
include(cmake/include_directories.cmake)
include_directories(/usr/local/include/asio)

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

# Include installation rules
include(cmake/install_rules.cmake)

# Include build types
include(cmake/build_types.cmake)