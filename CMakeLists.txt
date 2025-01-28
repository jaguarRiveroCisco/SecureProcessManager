cmake_minimum_required(VERSION 3.10)

project(DynamicControlOfChildProcesses)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(
    include/consolecontroller
    include/maincontroller
    include/processcontroller
    include/processmonitors
    include/process
    include/process/networkprocess
    include/process/systemprocess
    include/synchro
    include/tools
    include/logger
    include/networkprocess
    include/systemprocess
    /opt/homebrew/Cellar/asio/1.30.2/include  # Add the ASIO include directory
)


add_executable(ProcessController 
# Main source file
src/ProcessController.cpp 

# Process sources
src/process/process_simulator.cpp
src/process/process.cpp
src/process/base_process.cpp

# Network sources
src/process/networkprocess/network_process.cpp

# System sources
src/process/systemprocess/system_process.cpp

# ProcessController sources
src/processcontroller/process_controller.cpp

# Process Monitors
src/processmonitors/process_monitor.cpp
src/processmonitors/system_monitor.cpp

# Console Controller sources
src/consolecontroller/console_control.cpp
src/consolecontroller/cli_controller.cpp

# Main controller
src/maincontroller/main_controller.cpp

# Tools
src/tools/timestamp.cpp
src/tools/random_stuff.cpp
src/tools/time_manager.cpp
src/tools/thread_controller.cpp

# synchronization
src/synchro/communicator.cpp
src/synchro/messenger.cpp
src/synchro/semaphore_guard.cpp
src/synchro/synchro.cpp

# Logger
src/logger/logger.cpp
src/logger/console_logger.cpp
src/logger/file_logger.cpp
src/logger/logger_instance.cpp
)