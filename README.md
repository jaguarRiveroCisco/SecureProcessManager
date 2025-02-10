# Process Management Program

## Overview

The Process Management Program is a C++ application designed to handle and monitor multiple child processes. It utilizes threading, inter-process communication (IPC), and synchronization to efficiently manage process creation and lifecycle events.

## Key Components

- **API**: Provides the main interface for interacting with the process management system.
- **Concurrency**: Handles communication and synchronization between processes.
- **Config**: Manages configuration settings for the application.
- **ConsoleController**: Manages console input and output for controlling the application.
- **Logger**: Provides logging functionalities to track application events.
- **MainController**: Coordinates the overall process management.
- **Process**: Contains classes and utilities for managing individual processes.
- **ProcessController**: Manages the registry and control of processes.
- **ProcessMonitors**: Monitors the status and health of processes.
- **Tools**: Provides utility functions and classes used throughout the application.

## Features

- **Dynamic Process Creation**: Spawns and manages multiple child processes.
- **Threaded Monitoring**: Uses threads to monitor the status of each child process.
- **IPC and Synchronization**: Ensures smooth communication and synchronization between processes.
- **Logging**: Logs important events and errors for debugging and monitoring.

## User Commands

- **`context`**: Display the current context.
- **`quit`**: Signals the program to gracefully quit at the next loop.
- **`term all`**: Terminate (SIGTERM) all processes and exit the program.
- **`term <pid>`**: Terminate (SIGTERM) a specific process by PID.
- **`int all`**: Interrupt (SIGINT) all processes and exit the program.
- **`int <pid>`**: Interrupt (SIGINT) a specific process by PID.
- **`kill all`**: Kill all (SIGKILL) processes and exit the program.
- **`kill <pid>`**: Kill a (SIGKILL) specific process by PID.
- **`display pids`**: Display all current PIDs.
- **`respawn on`**: Turn on respawn.
- **`respawn off`**: Turn off respawn.
- **`monitor on`**: Turn on monitoring: spawn monitoring threads.
- **`monitor off`**: Turn off monitoring: end monitoring threads.
- **`help`**: Display the help message with available commands.

### Building the Project

To build the project, you can use the provided `build.sh` script to clean and build the project. The script offers several options:

```bash
./build.sh
```
#### Script Options

1. **Clean Debug**: Clean the debug build.
2. **Clean Release**: Clean the release build.
3. **Clean Debug-Coverage**: Clean the debug-coverage build.
4. **Build Debug**: Build the debug configuration.
5. **Build Release**: Build the release configuration.
6. **Build Debug-Coverage**: Build the debug-coverage configuration.
7. **Clean and Build All**: Clean and build all configurations.

To use the script, simply run it and select the desired option from the menu.
## Project Structure

The project is organized into several directories, each containing specific components of the application:

- **`include`**: Contains header files for various modules.
  - **`consolecontroller`**: Headers for console control.
  - **`maincontroller`**: Headers for the main controller.
  - **`processcontroller`**: Headers for process control.
  - **`processmonitors`**: Headers for process monitoring.
  - **`process`**: Headers for process management.
    - **`networkprocess`**: Headers for network processes.
    - **`systemprocess`**: Headers for system processes.
  - **`concurrency`**: Headers for concurrency management.
  - **`tools`**: Utility headers.
  - **`logger`**: Headers for logging.
  - **`config`**: Headers for configuration management.
  - **`api`**: Headers for the API.
  - **`/opt/homebrew/Cellar/asio/1.30.2/include`**: ASIO library headers.

- **`src`**: Contains source files for the application.
  - **`process`**: Source files for process management.
    - **`networkprocess`**: Source files for network processes.
    - **`systemprocess`**: Source files for system processes.
  - **`processcontroller`**: Source files for process control.
  - **`processmonitors`**: Source files for process monitoring.
  - **`consolecontroller`**: Source files for console control.
  - **`maincontroller`**: Source files for the main controller.
  - **`tools`**: Utility source files.
  - **`concurrency`**: Source files for concurrency management.
  - **`logger`**: Source files for logging.
  - **`api`**: Source files for the API.
  - **`config`**: Source files for configuration management.

- **`tests`**: Contains test files for the application.

## Building the Library and Executables

The project builds a static library and two executables:

- **Library**: `ProcessControllerLib`
  - Contains common source files used across the application.

- **Main Executable**: `ProcessController`
  - The main application executable.

- **Test Executable**: `ProcessControllerTests`
  - Contains test cases for various components of the application.


## Linking Libraries

The main executable and test executable link against the `ProcessControllerLib` library. The test executable also links against the Google Test libraries and pthread.

## Adding Google Test

Google Test is required for running the tests. Ensure it is installed and available on your system. The CMake configuration will find and include the necessary Google Test directories.

## Compiler Flags

The project uses the following compiler flags:
- `-Wall`: Enable all compiler's warning messages.
- `-Wunused-variable`: Warn about unused variables.
- `-Wunused-function`: Warn about unused functions.

## Setting C++ Standard

The project is set to use the C++20 standard.
## Running the Program

Execute the program using the command below. This will run the executable located in the `build` directory:

# Creating a Simple Program to Leverage the Library

This guide will help you set up a new project that utilizes the `ProcessControllerLib` library.

## Prerequisites

0. **Install the Library**: Ensure that the library is installed by executing `build.sh` in the `ProcessController` directory.

## Steps to Set Up the New Project

**Make sure you have executed build.sh in the ProcessController directory to install the library.**

### 1. Create the Directory Structure

```sh
mkdir -p ~/dev/programs/testprogs/progcontrol
cd ~/dev/programs/testprogs/progcontrol
mkdir src include
```

```cmake

cmake_minimum_required(VERSION 3.30)
project(ProgControl)

set(CMAKE_CXX_STANDARD 20)

# Base directory for installed headers
set(BASE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../ProcessController/install/include)

# Include the installed headers
include_directories(
${BASE_INCLUDE_DIR}
${BASE_INCLUDE_DIR}/consolecontroller
${BASE_INCLUDE_DIR}/maincontroller
${BASE_INCLUDE_DIR}/processcontroller
${BASE_INCLUDE_DIR}/processmonitors
${BASE_INCLUDE_DIR}/process
${BASE_INCLUDE_DIR}/process/networkprocess
${BASE_INCLUDE_DIR}/process/systemprocess
${BASE_INCLUDE_DIR}/concurrency
${BASE_INCLUDE_DIR}/tools
${BASE_INCLUDE_DIR}/logger
${BASE_INCLUDE_DIR}/networkprocess
${BASE_INCLUDE_DIR}/systemprocess
${BASE_INCLUDE_DIR}/config
${BASE_INCLUDE_DIR}/api
)

# Link the installed library
link_directories(${CMAKE_SOURCE_DIR}/../../ProcessController/install/lib)

# Create an executable
add_executable(prog_control src/progcontrol.cpp)

# Link the ProcessControllerLib library
target_link_libraries(prog_control ProcessControllerLib)

```

### 2. Create the Main Program File

```cpp

### 3. Create the Main Program File

#include "api/api.h"
#include "consolecontroller/console_control.h"
#include "tools/nap_time.h"
#include <thread>
#include "consolecontroller/console_loop.h"

void displayCompilationInfo(const char *appName)
{
    std::cout << "*******************************************" << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*  Application Name: " << appName << std::endl;
    std::cout << "*  Compiled on: " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*******************************************" << std::endl;
}

auto main(int argc, char *argv[]) -> int
{
    displayCompilationInfo(argv[0]);

    int         numProcesses = 4;
    std::string processType  = "simul";
    static tools::ConsoleLogger cl;

    cli::driver::parseArguments(argc, argv, numProcesses, processType, cl);

    cli::driver::printContext(numProcesses, processType);

    cli::driver::printCommands(); // Call to printHelp

    api::initialize(numProcesses, processType);

    cli::driver::consoleLoop();

    api::execute();

    cli::driver::consoleLoop(false);

    std::this_thread::sleep_for(std::chrono::milliseconds(tools::NapTimeMs::LONG));

    cl.logInfo("[EXITING] Main process exiting");

    return 0;
}

```

### 3. Build the Project

```sh

cd ~/dev/programs/testprogs/progcontrol
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target prog_control -j 6

 ```

### 4. Run the Program

```sh
./prog_control
```

### Command-Line Options for the sample program

- **`-n <number of processes>`**:  
  Specify the number of child processes to create. Defaults to 4 if not provided.  
  Example: `-n 5` to create 5 child processes.

- **`-t <process type>`**:  
  Specify the process type ('real', 'network', 'system', or 'simul'). Defaults to 'simul'.  
  Example: `-t real` to use real processes instead of simulated ones.

- **`-s <respawn>`**:  
  Enable or disable respawn (0 for off, 1 for on).  
  Example: `-s 1` to enable respawn.

- **`-l <logging type>`**:  
  Specify the logging type ('console' or 'file'). Defaults to 'console'.  
  Example: `-l file` to log to a file.

- **`-T <nap time type>`**:  
  Specify the nap time type ('MS', 'SEC', 'MIN'). Defaults to 'MS'.  
  Example: `-T SEC` to set nap time type to seconds.

- **`-c <configuration file path>`**:  
  Specify the path to the configuration file.  
  Example: `-c /path/to/config/file` to use the specified configuration file.

- **`-h`**:  
  Display usage information and exit the program.  
  Example: `-h` to show the help message with usage instructions.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Authors

- jarivero@cisco.com

## Copyright

© 2025 Cisco Systems, Inc. All rights reserved. Unauthorized use, distribution, and reproduction are strictly prohibited.


