# Ensure CMAKE_INSTALL_PREFIX is set by the build script
if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
else()
    message(FATAL_ERROR "CMAKE_INSTALL_PREFIX should be set by the build script.")
endif()

# Installation rules
install(TARGETS SecureProcessManager SecureProcessManagerLib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

# Optionally install header files
install(DIRECTORY include/ DESTINATION include)