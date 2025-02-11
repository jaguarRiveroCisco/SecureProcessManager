#!/usr/bin/env bash

# Function to display help information
show_help() {
    echo "Usage: $0 [OPTION]"
    echo "Options:"
    echo "  -c, --clean [debug|release|debug-coverage]    Clean the specified build type."
    echo "  -b, --build [debug|release|debug-coverage]    Build the specified build type."
    echo "  -i, --install [debug|release|debug-coverage]  Install the specified build type."
    echo "  -a, --all                                     Clean and build all types."
    echo "  -h, --help                                    Show this help message."
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
        cmake -DCMAKE_INSTALL_PREFIX=../install ..
        cmake --build . --target install -j 6
    )
    echo "Install finished for ${build_type} build."
}

# Check if build directories exist
if [ ! -d "cmake-build-debug" ] || [ ! -d "cmake-build-release" ] || [ ! -d "cmake-build-debug-coverage" ]; then
    echo "Build directories do not exist. Configuring project with CMake..."
    cmake -S . -B cmake-build-debug
    cmake -S . -B cmake-build-release
    cmake -S . -B cmake-build-debug-coverage
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
    read -r -p "Enter your choice [1-10]: " choice

    case $choice in
        1) clean_build "debug" ;;
        2) clean_build "release" ;;
        3) clean_build "debug-coverage" ;;
        4) compile_build "debug" ;;
        5) compile_build "release" ;;
        6) compile_build "debug-coverage" ;;
        7)
            clean_build "debug"
            compile_build "debug"
            clean_build "release"
            compile_build "release"
            clean_build "debug-coverage"
            compile_build "debug-coverage"
            ;;
        8) install_build "debug" ;;
        9) install_build "release" ;;
        10) install_build "debug-coverage" ;;
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
            -a|--all)
                clean_build "debug"
                compile_build "debug"
                clean_build "release"
                compile_build "release"
                clean_build "debug-coverage"
                compile_build "debug-coverage"
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