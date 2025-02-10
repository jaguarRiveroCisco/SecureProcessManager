#!/usr/bin/env bash

# Function to clean the build
clean_build() {
    local build_type=$1
    echo "Cleaning ${build_type} build..."
    cmake --build cmake-build-${build_type} --target clean -j 6
    echo "Clean finished for ${build_type} build."
}

# Function to compile the build
compile_build() {
    local build_type=$1
    echo "Building ${build_type} build..."
    cmake --build cmake-build-${build_type} --target all -j 6
    echo "Build finished for ${build_type} build."
}

# Check if the build directories exist
if [ ! -d "cmake-build-debug" ] || [ ! -d "cmake-build-release" ] || [ ! -d "cmake-build-debug-coverage" ]; then
    echo "Build directories do not exist. Please configure your project with CMake first."
    exit 1
fi

# Main script execution
echo "Select an option:"
echo "1) Clean Debug"
echo "2) Clean Release"
echo "3) Clean Debug-Coverage"
echo "4) Build Debug"
echo "5) Build Release"
echo "6) Build Debug-Coverage"
echo "7) Clean and Build All"
read -p "Enter your choice [1-7]: " choice

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
    *)
        echo "Invalid choice. Exiting."
        exit 1
        ;;
esac

exit 0
