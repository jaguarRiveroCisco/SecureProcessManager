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

# Add custom targets for cleaning, building, and installing
add_custom_target(clean-debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target clean
    COMMENT "Cleaning debug build..."
)

add_custom_target(build-debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target all
    COMMENT "Building debug build..."
)

add_custom_target(install-debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target install
    COMMENT "Installing debug build..."
)

add_custom_target(clean-release
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-release --target clean
    COMMENT "Cleaning release build..."
)

add_custom_target(build-release
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-release --target all
    COMMENT "Building release build..."
)

add_custom_target(install-release
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-release --target install
    COMMENT "Installing release build..."
)

add_custom_target(clean-debug-coverage
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target clean
    COMMENT "Cleaning debug-coverage build..."
)

add_custom_target(build-debug-coverage
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target all
    COMMENT "Building debug-coverage build..."
)

add_custom_target(install-debug-coverage
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target install
    COMMENT "Installing debug-coverage build..."
)

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