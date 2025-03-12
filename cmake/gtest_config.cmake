# gtest_config.cmake

# Add Google Test
find_package(GTest REQUIRED)
if(NOT GTest_FOUND)
    message(FATAL_ERROR "Google Test not found. Please install Google Test.")
endif()

include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(SecureProcessManagerTests
    tests/test_main.cpp
    tests/test_logger.cpp
    tests/tests_maincontroller.cpp
    tests/test_base_process.cpp
    tests/test_string_tools.cpp
    tests/test_control_loop.cpp
    tests/test_system_process.cpp
)

# Link the library to the test executable
target_link_libraries(SecureProcessManagerTests SecureProcessManagerLib ${GTEST_LIBRARIES} pthread)