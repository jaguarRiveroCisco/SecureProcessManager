cmake_minimum_required(VERSION 3.10)

project(DynamicControlOfChildProcesses)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(
    include/controller
    include/process
    include/synchro
)


add_executable(ProcessController 
src/waitPidProc.cpp 
src/process/messenger.cpp 
src/process/process_base.cpp
src/process/process_helper.cpp
src/process/simul_process.cpp
src/process/process.cpp
src/controller/process_handler.cpp
src/controller/process_control.cpp
src/controller/controller_base.cpp
)