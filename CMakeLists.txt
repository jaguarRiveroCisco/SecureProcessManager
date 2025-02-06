cmake_minimum_required(VERSION 3.10)

               project(DynamicControlOfChildProcesses)

               set(CMAKE_CXX_STANDARD 20)

               # Add -Wall flag for the C++ compiler
               set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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

               add_executable(
                       ProcessController
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
                       src/consolecontroller/console_loop.cpp

                       # Main controller
                       src/maincontroller/main_controller.cpp
                       src/maincontroller/main_controller_createthreads.cpp
                       src/maincontroller/main_controller_terminatethreads.cpp

                       # Tools
                       src/tools/timestamp.cpp
                       src/tools/random_stuff.cpp
                       src/tools/time_manager.cpp
                       src/tools/thread_controller.cpp
                       src/tools/string_tools.cpp

                       # synchronization
                       src/concurrency/communicator.cpp
                       src/concurrency/messenger.cpp
                       src/concurrency/synchro.cpp

                       # Logger
                       src/logger/logger.cpp
                       src/logger/console_logger.cpp
                       src/logger/file_logger.cpp
                       src/logger/logger_instance.cpp
                       src/tools/process_status.cpp
                       src/processcontroller/ProcessRegistry.cpp

                       # API
                       src/api/api.cpp

                       #configuration
                       src/config/ConfigReader.cpp
                       )