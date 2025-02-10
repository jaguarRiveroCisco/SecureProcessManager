# common_sources.cmake

set(COMMON_SOURCES
    src/process/process_simulator.cpp
    src/process/process.cpp
    src/process/base_process.cpp
    src/process/networkprocess/network_process.cpp
    src/process/systemprocess/system_process.cpp
    src/process/systemprocess/file_descriptor.cpp

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
