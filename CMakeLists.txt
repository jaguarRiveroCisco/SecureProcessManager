cmake_minimum_required(VERSION 3.10)

project(MyCMakeProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(waitPidProc 
src/waitPidProc.cpp 
src/messenger.cpp 
src/process_base.cpp
src/process_helper.cpp
src/simul_process.cpp
)