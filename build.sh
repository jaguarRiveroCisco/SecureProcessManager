#!/usr/bin/env bash

set -e # Exit immediately if a command exits with a non-zero status.


    INSTALL_PREFIX="../install"

    # Function to display help information
    show_help() {
        echo "Usage: $0 [OPTION]"
        echo "Options:"
        echo "  -c, --clean [debug|release|debug-coverage]    Clean the specified build type."
        echo "  -b, --build [debug|release|debug-coverage]    Build the specified build type."
        echo "  -i, --install [debug|release|debug-coverage]  Install the specified build type."
        echo "  -t, --test [debug|release|debug-coverage]     Execute tests for the specified build type."
        echo "  -a, --all                                     Clean, build, and install all types."
        echo "  -h, --help                                    Show this help message."
        echo "  -z, --initialize                              Initialize the build directories."
        echo "  -T, --test-all                                Execute tests for all build types."
        echo "  -d, --debug                                   Debug the ProcessController."
        exit 0
    }

    # Function to clean the build
    clean_build() {
        local build_type=$1
        echo "Cleaning ${build_type} build..."
        cmake --build cmake-build-"${build_type}" --target clean -j 6
        echo "Clean finished for ${build_type} build."
    }

    # Function to compile the build
    compile_build() {
        local build_type=$1
        echo "Building ${build_type} build..."
        cmake --build cmake-build-"${build_type}" --target all -j 6
        echo "Build finished for ${build_type} build."
    }

    # Function to install the build
    install_build() {
        local build_type=$1
        echo "Installing ${build_type} build..."
        (
            cd cmake-build-"${build_type}" || exit
            cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" ..
            cmake --build . --target install -j 6
        )
        echo "Install finished for ${build_type} build."
    }

    # Function to execute tests
    execute_tests() {
        local build_type=$1
        echo "Executing tests for ${build_type} build..."
        (
            cd cmake-build-"${build_type}" || exit
            ./ProcessControllerTests
        )
        echo "Tests executed for ${build_type} build."
    }

build_types=("debug" "release" "debug-coverage")

clean_all() {
    for build_type in "${build_types[@]}"; do
        clean_build "$build_type"
    done
    echo "Clean finished for all builds."
}

build_all() {
    for build_type in "${build_types[@]}"; do
        compile_build "$build_type"
    done
    echo "Build finished for all builds."
}

install_all() {
    for build_type in "${build_types[@]}"; do
        install_build "$build_type"
    done
    echo "Install finished for all builds."
}

test_all() {
    for build_type in "${build_types[@]}"; do
        execute_tests "$build_type"
    done
    echo "Tests executed for all builds."
}

    # Function to initialize build directories
    initialize() {
        echo "Initializing build directories with install prefix: ${INSTALL_PREFIX}..."
        rm -rf cmake-build-debug
        rm -rf cmake-build-release
        rm -rf cmake-build-debug-coverage
        cmake -S . -B cmake-build-debug -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Debug
        cmake -S . -B cmake-build-release -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Release
        cmake -S . -B cmake-build-debug-coverage -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Debug
        echo "Initialization finished."
    }

    # Function to debug the ProcessController
    debug_build() {
        echo "Debugging ProcessController..."
        (
            cd cmake-build-debug || exit
            cmake -DCMAKE_BUILD_TYPE=Debug ..
            make
            gdb ./ProcessController
        )
    }

    # Check if build directories exist
    if [ ! -d "cmake-build-debug" ] || [ ! -d "cmake-build-release" ] || [ ! -d "cmake-build-debug-coverage" ]; then
        echo "Build directories do not exist. Configuring project with CMake..."
        initialize
    fi

    # Function to enter interactive mode
    interactive_mode() {
        echo "Select an option:"
        echo "1) Clean Debug"
        echo "2) Clean Release"
        echo "3) Clean Debug-Coverage"
        echo "4) Build Debug"
        echo "5) Build Release"
        echo "6) Build Debug-Coverage"
        echo "7) Clean and Build All"
        echo "8) Install Debug"
        echo "9) Install Release"
        echo "10) Install Debug-Coverage"
        echo "11) Install All"
        echo "12) Initialize"
        echo "13) Execute Tests Debug"
        echo "14) Execute Tests Release"
        echo "15) Execute Tests Debug-Coverage"
        echo "16) Execute Tests All"
        echo "17) Debug ProcessController"
        read -r -p "Enter your choice [1-17]: " choice

        case $choice in
            1) clean_build "debug" ;;
            2) clean_build "release" ;;
            3) clean_build "debug-coverage" ;;
            4) compile_build "debug" ;;
            5) compile_build "release" ;;
            6) compile_build "debug-coverage" ;;
            7)
                clean_all
                build_all
                ;;
            8) install_build "debug" ;;
            9) install_build "release" ;;
            10) install_build "debug-coverage" ;;
            11) install_all ;;
            12) initialize ;;
            13) execute_tests "debug" ;;
            14) execute_tests "release" ;;
            15) execute_tests "debug-coverage" ;;
            16) test_all ;;
            17) debug_build ;;
            *)
                echo "Invalid choice. Exiting."
                exit 1
                ;;
        esac
    }

    # Parse command-line options
    if [ "$#" -eq 0 ]; then
        # No command-line options provided; enter interactive mode
        interactive_mode
    else
        while [[ $# -gt 0 ]]; do
            case $1 in
                -c|--clean)
                    clean_build "$2"
                    shift 2
                    ;;
                -b|--build)
                    compile_build "$2"
                    shift 2
                    ;;
                -i|--install)
                    install_build "$2"
                    shift 2
                    ;;
                -t|--test)
                    execute_tests "$2"
                    shift 2
                    ;;
                -a|--all)
                    clean_all
                    build_all
                    install_all
                    shift
                    ;;
                -z|--initialize)
                    initialize
                    shift
                    ;;
                -T|--test-all)
                    test_all
                    shift
                    ;;
                -d|--debug)
                    debug_build
                    shift
                    ;;
                -h|--help)
                    show_help
                    ;;
                *)
                    echo "Unknown option $1"
                    show_help
                    ;;
            esac
        done
    fi

    exit 0