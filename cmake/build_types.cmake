# Define a list of build types
set(BUILD_TYPES debug release debug-coverage)

# Iterate over build types to create custom targets
foreach(BUILD_TYPE IN LISTS BUILD_TYPES)
    string(TOUPPER ${BUILD_TYPE} BUILD_TYPE_UPPER)

    add_custom_target(clean-${BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-${BUILD_TYPE} --target clean
        COMMENT "Cleaning ${BUILD_TYPE} build..."
    )

    add_custom_target(build-${BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-${BUILD_TYPE} --target all
        COMMENT "Building ${BUILD_TYPE} build..."
    )

    add_custom_target(install-${BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-${BUILD_TYPE} --target install
        COMMENT "Installing ${BUILD_TYPE} build..."
    )
endforeach()

# Add a custom target to clean and build all types
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-release --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target clean
    COMMENT "Cleaning all builds..."
)

add_custom_target(build-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-release --target all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target all
    COMMENT "Building all builds..."
)

add_custom_target(install-all
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-debug --target install
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-release --target install
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install --build ${CMAKE_BINARY_DIR}/cmake-build-debug-coverage --target install
    COMMENT "Installing all builds..."
)