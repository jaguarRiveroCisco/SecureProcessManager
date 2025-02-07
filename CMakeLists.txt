cmake_minimum_required(VERSION 3.10)

    project(DynamicControlOfChildProcesses)

    set(CMAKE_CXX_STANDARD 20)

    # Add -Wall flag for the C++ compiler
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-variable -Wunused-function")

# Include directories
include_directories(
        include/consolecontroller
        include/maincontroller
        include/processcontroller
        include/processmonitors
        include/process
        include/process/networkprocess
        include/process/systemprocess
        include/concurrency
        include/tools
        include/logger
        include/networkprocess
        include/systemprocess
        include/config
        include/api
        /opt/homebrew/Cellar/asio/1.30.2/include  # Add the ASIO include directory
        )

    # Add Google Test
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Define common source files
    set(COMMON_SOURCES
        src/process/process_simulator.cpp
        src/process/process.cpp
        src/process/base_process.cpp
        src/process/networkprocess/network_process.cpp
        src/process/systemprocess/system_process.cpp

        src/processcontroller/process_controller.cpp
        src/processcontroller/ProcessRegistry.cpp

        src/processmonitors/process_monitor.cpp
        src/processmonitors/system_monitor.cpp

        src/consolecontroller/console_control.cpp
        src/consolecontroller/cli_controller.cpp
        src/consolecontroller/console_loop.cpp

        src/maincontroller/main_controller.cpp
        src/maincontroller/main_controller_createthreads.cpp
        src/maincontroller/main_controller_terminatethreads.cpp

        src/tools/timestamp.cpp
        src/tools/random_stuff.cpp
        src/tools/time_manager.cpp
        src/tools/thread_controller.cpp
        src/tools/string_tools.cpp
        src/tools/process_status.cpp

        src/concurrency/communicator.cpp
        src/concurrency/messenger.cpp
        src/concurrency/synchro.cpp

        src/logger/logger.cpp
        src/logger/console_logger.cpp
        src/logger/file_logger.cpp
        src/logger/logger_instance.cpp

        src/api/api.cpp

        src/config/ConfigReader.cpp
    )

    # Create a library
    add_library(ProcessControllerLib STATIC ${COMMON_SOURCES})

    # Main executable
    add_executable(ProcessController src/ProcessController.cpp)

    # Link the library to the main executable
    target_link_libraries(ProcessController ProcessControllerLib)

    # Test executable
    add_executable(ProcessControllerTests
        tests/test_main.cpp
        tests/test_logger.cpp
    )

    # Link the library to the test executable
    target_link_libraries(ProcessControllerTests ProcessControllerLib ${GTEST_LIBRARIES} pthread)