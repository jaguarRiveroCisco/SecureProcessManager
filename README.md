# Process Management Program

## Overview

The Process Management Program is a C++ application designed to handle and monitor multiple child processes. It utilizes threading, inter-process communication (IPC), and synchronization to efficiently manage process creation and lifecycle events.

## Introduction

This program can spawn and monitor child processes. It supports two types of processes:
- **System Processes**: Require a configuration/parameters file.
- **Other Processes**: Do not require a configuration/parameters file.

The program can be executed in two modes:
- **Console-Driven**: The user interacts with the program via the console.
- **Library Mode**: The user can integrate the library into another program to manage processes.

## Requirements

- cmake 3.30 or higher
- g++ 12.0 or higher
- Google Test 1.11.0 or higher
- ASIO 1.30.2 or higher

```bash
sudo apt-get update
sudo apt-get install -y wget gnupg
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc | gpg --dearmor -o kitware-archive-keyring.gpg
sudo mv kitware-archive-keyring.gpg /etc/apt/trusted.gpg.d/
echo 'deb https://apt.kitware.com/ubuntu/ jammy main' | sudo tee /etc/apt/sources.list.d/kitware.list
sudo apt-get install cmake

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get install g++-12
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 60

sudo apt-get install libasio-dev

sudo apt-get install libgtest-dev
```

### Features

- **Dynamic Process Creation**: Spawns and manages multiple child processes.
- **Threaded Monitoring**: Uses threads to monitor the status of each child process.
- **IPC and Synchronization**: Ensures smooth communication and synchronization between processes.
- **Logging**: Logs important events and errors for debugging and monitoring.

### Types of Processes

#### Already Implemented

- **System Processes**: Execute system commands and require a configuration file with the executable path and parameters. No user extension needed, only a configuration file.
- **Network Processes**: Perform network activities and do not require a configuration file. Currently simulated by the program.
- **Simulated Processes**: Simulated by the program for testing purposes. Do not require a configuration file.
- **Real Processes**: Simulate real-life processes that do not finish randomly. They need to be terminated/killed by the user.

#### To Be Extended by the Developer

- **Custom Processes**: Created by the user. The user must provide the implementation of the process and, if required, the monitor. See the example below.

## Console-driven User Commands

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

##### NOTE: The program will not exit until all processes are terminated.
##### NOTE: Most command line options can be accessed via the api (see api.h).

### Command-Line Options

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

### Building the Project

To build the project, you can use the provided `build.sh` script to clean and build the project. The script offers several options:

```bash
./build.sh
```
#### Script Options

- **Clean Debug**: Clean the debug build.
- **Clean Release**: Clean the release build.
- **Clean Debug-Coverage**: Clean the debug-coverage build.
- **Build Debug**: Build the debug configuration.
- **Build Release**: Build the release configuration.
- **Build Debug-Coverage**: Build the debug-coverage configuration.
- **Clean and Build All**: Clean and build all configurations.
- **Install Debug**: Install the debug build.
- **Install Release**: Install the release build.
- **Install Debug-Coverage**: Install the debug-coverage build.

##### NOTE: To use the script, simply run it and select the desired option from the menu. Alternatively it can be executed by passing the desired option as argument (type ./build.sh -h for help).

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
- `-Wall`: Enable all compilers warning messages.
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
#include "api/api.h"
#include "consolecontroller/console_control.h"
#include "tools/nap_time.h"
#include <thread>
#include "consolecontroller/console_loop.h"
#include "process/process.h"
#include "processmonitors/process_monitor.h"

struct CustomMonitor final : process::ProcessMonitor
{
};

struct CustomProcess final : process::Process
{

};


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
    std::string processType  = "custom";
    static tools::ConsoleLogger cl;

    cli::driver::parseArguments(argc, argv, numProcesses, processType, cl);

    cli::driver::printContext(numProcesses, processType);

    cli::driver::printCommands(); // Call to printHelp

    // Initialize controller with new type and number of processes
    api::registerHandler<CustomMonitor,CustomProcess>(processType);

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
./prog_control  -t system -n 1 -c /Users/jrivero/dev/programs/ProcessController/exampleConfigFile.conf
```

## Examples of command line usage of the sample program

```sh
 ./ProcessController -t system -n 2
 ./ProcessController -l file
 ./ProcessController -c path/to/ConfigFile.conf -t system -l file

```
### Configuration File

The configuration file specifies the executable and its parameters, which are used by the \`Arguments\` class to populate the arguments vector. A sample configuration file might look like this:
```sh
process_file=/Users/jrivero/dev/programs/testprogs/lengthy/lengthy_process
s=30
v=1
#username=admin
#password=securepassword123
#server=localhost
#port=8080
#database=mydatabase
#max_connections=77
#timeout=33
#log_level=debug
```

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

### Appendix

#### Using the Arguments Class to Manage Process Execution

We leverage the \`Arguments\` class to handle the configuration and
parameterization of the posix_spawn-created processes, making it easier to handle the process' parameters.

**Arguments Class:**

- **Purpose**: Manages the command-line arguments for the process being executed.
- **Functionality**:
  - Reads configuration from a file.
  - Populates the arguments vector (\`args\`) with the executable path and its parameters.
  - Extracts and stores the filename without its extension for logging purposes.

**SystemProcess Class:**

- **Purpose**: Represents a system process that can be started and managed.
- **work Method**: Initiates a new thread to spawn a child process using the \`SpawnChild\` class.

**SpawnChild Class:**

- **Purpose**: Encapsulates the logic for spawning a child process with \`posix\_spawn\`.
- **Constructor**:
  - Initializes file actions and attributes for process spawning.
  - Redirects stdout and stderr to a uniquely named log file.
  - Converts command-line arguments to C-style strings for \`posix\_spawn\`.
- **executeCommand Method**:
  - Executes the command using \`posix\_spawn\`.
  - Waits for the process to complete and captures its exit status.
  - Notifies the parent process of completion and handles logging.


## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Authors

- jarivero@cisco.com

## Copyright

© 2025 Cisco Systems, Inc. All rights reserved. Unauthorized use, distribution, and reproduction are strictly prohibited.


