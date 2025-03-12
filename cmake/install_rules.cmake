# Set default install prefix if not specified
if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")

# Installation rules
install(TARGETS SecureProcessManager SecureProcessManagerLib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

# Optionally install header files
install(DIRECTORY include/ DESTINATION include)