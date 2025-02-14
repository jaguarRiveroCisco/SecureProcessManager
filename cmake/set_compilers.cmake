# set_compilers.cmake

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

