cmake_minimum_required(VERSION 3.30)

project(DynamicControlOfChildProcesses)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add -Wall flag for the C++ compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wunused-function")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# Check if the platform is not macOS
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Setting compilers.")
    # Check if the desired compilers exist
    find_program(GCC_PATH gcc PATHS /usr/bin NO_DEFAULT_PATH)
    find_program(GXX_PATH g++ PATHS /usr/bin NO_DEFAULT_PATH)

    # Set the C compiler if not already set correctly
    if(NOT CMAKE_C_COMPILER STREQUAL "/usr/bin/gcc")
        if(GCC_PATH)
            set(CMAKE_C_COMPILER ${GCC_PATH} CACHE STRING "C compiler" FORCE)
        else()
            message(FATAL_ERROR "GCC not found at /usr/bin/gcc.")
        endif()
    endif()

    # Set the C++ compiler if not already set correctly
    if(NOT CMAKE_CXX_COMPILER STREQUAL "/usr/bin/g++")
        if(GXX_PATH)
            set(CMAKE_CXX_COMPILER ${GXX_PATH} CACHE STRING "C++ compiler" FORCE)
        else()
            message(FATAL_ERROR "G++ not found at /usr/bin/g++.")
        endif()
    endif()

else()
    message(STATUS "Skipping compiler settings on macOS.")
endif()

# Print the compilers being used
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")


# Continue with the rest of your CMake configuration
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

# Set default install prefix if not specified
if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

# Installation rules
install(TARGETS ProcessController ProcessControllerLib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

# Optionally install header files
install(DIRECTORY include/ DESTINATION include)

# Define a list of build types
set(BUILD_TYPES debug release debug-coverage)

# Iterate over build types to create custom targets
foreach(BUILD_TYPE IN LISTS BUILD_TYPES)
    string(TOUPPER ${BUILD_TYPE} BUILD_TYPE_UPPER)

    add_custom_target(clean-${BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-${BUILD_TYPE} --target clean
        COMMENT "Cleaning ${BUILD_TYPE} build..."
    )

    add_custom_target(build-${BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-${BUILD_TYPE} --target all
        COMMENT "Building ${BUILD_TYPE} build..."
    )

    add_custom_target(install-${BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-${BUILD_TYPE} --target install
        COMMENT "Installing ${BUILD_TYPE} build..."
    )
endforeach()

# Add a custom target to clean and build all types
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-release --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target clean
    COMMENT "Cleaning all builds..."
)

add_custom_target(build-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-release --target all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target all
    COMMENT "Building all builds..."
)

add_custom_target(install-all
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target install
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-release --target install
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target install
    COMMENT "Installing all builds..."
)