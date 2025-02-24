# Process Management Library and Program

## Overview

The Process Management Library and Program is a C++ library along with a C++ application designed to spawn, control and 
monitor multiple child processes. 

The library leverages threading, inter-process communication (IPC), and synchronization mechanisms to manage process creation and 
lifecycle events. The code comes with an example program to utilize for library testing and integration.

## Introduction

This library is capable of spawning and monitoring child processes. It supports two categories of processes:
- **System Processes**: (```posix_spawn() ```) These processes require a configuration or parameters file to function.
- **Other Processes**: These processes require to extend the functionality of the base classes.

The library and its program can be executed in two distinct modes:
- **Console-Driven Mode**: Users interact with the library via the ProcessController program directly through the console interface.
- **Library Mode**: The library can be integrated into other applications, allowing users to manage processes programmatically.

## Requirements and dependencies

- cmake 3.30 or higher
- g++ 12.0 or higher
- Google Test 1.11.0 or higher
- ASIO 1.30.2 or higher

### Installing Dependencies

```bash
#!/bin/bash

# Update package list
sudo apt-get update

# Install basic tools
sudo apt-get install -y wget gnupg

# Install CMake \(Option A: from Kitware APT repository\)
sudo rm /etc/apt/trusted.gpg.d/kitware-archive.gpg
wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc | gpg --dearmor -o kitware-archive-keyring.gpg
sudo mv kitware-archive-keyring.gpg /etc/apt/trusted.gpg.d/
sudo chmod 644 /etc/apt/trusted.gpg.d/kitware-archive-keyring.gpg
sudo apt update

sudo apt-get install cmake

# Install GTest
sudo apt-get install libgtest-dev

# Add Toolchain PPA for latest GCC and G++
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-12

# Set G++ 12 as default
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 60

# Install ASIO
sudo apt-get remove --purge libasio-dev
git config --global http.postBuffer 524288000
git clone --depth 1 https://github.com/chriskohlhoff/asio.git
# ...or
git clone git@github.com:chriskohlhoff/asio.git
# then...
cd asio
git fetch --unshallow
#..or
git fetch --depth 1 origin master

sudo mkdir -p /usr/local/include/asio
sudo cp -r asio/include/* /usr/local/include/asio
sudo chmod -R a+r /usr/local/include/asio
sudo chown -R root:root /usr/local/include/asio
sudo chmod -R a+rx /usr/local/include

echo "Installation complete."
```
## Features

- **Dynamic Process Creation**: Capable of spawning and managing multiple child processes efficiently.
- **Threaded Monitoring**: Utilizes threads to continuously monitor the status of each child process.
- **IPC and Synchronization**: Facilitates smooth communication and synchronization between processes.
- **Logging**: Records important events and errors to aid in debugging and monitoring.

## Types of Processes

### Existing Implementations

- ***System Processes***: (```posix_spawn()```) These execute predefined system commands and rely on a configuration file to specify the executable path and necessary parameters. Once the configuration file is provided, no further user intervention is required.

- **Network Processes**: These engage in network-related activities and are currently simulated by the program, meaning they do not require a configuration file for operation. A sample network process is provided.

- **Simulated Processes**: These serve as test cases within the program, allowing examination of process management without actual execution. They are completely simulated and do not require a configuration file.

- **Real Processes**: These represent ongoing, real-world operations that do not conclude automatically. These processes must be manually terminated or stopped by the user using the provided interface, or externally, when necessary. Extend from this class for your processes. See below for Custom classes.

### Custom Development

- **Custom Processes**: Developers have the flexibility to create custom processes by extending the base functionality (extend from Process class and ProcessMonitor, see example below). This includes implementing specific behavior and, if necessary, integrating a custom monitor to meet unique requirements.

## ProcessController Program Console-Driven User Commands

- **`context`**: Displays the current context of the program.
- **`quit`**: Gracefully exits the program during the next loop iteration.
- **`term all`**: Sends a SIGTERM signal to terminate all processes and exits the program.
- **`term <pid>`**: Sends a SIGTERM signal to terminate a specific process by its PID.
- **`int all`**: Sends a SIGINT signal to interrupt all processes and exits the program.
- **`int <pid>`**: Sends a SIGINT signal to interrupt a specific process by its PID.
- **`kill all`**: Sends a SIGKILL signal to terminate all processes and exits the program.
- **`kill <pid>`**: Sends a SIGKILL signal to terminate a specific process by its PID.
- **`display pids`**: Lists all current process IDs.
- **`respawn on`**: Enables automatic respawning of processes.
- **`respawn off`**: Disables automatic respawning of processes.
- **`monitor on`**: Activates monitoring by spawning monitoring threads.
- **`monitor off`**: Deactivates monitoring by terminating monitoring threads.
- **`help`**: Shows the help message with a list of available commands.

**NOTE**: The program will not exit until all processes are terminated.  
**NOTE**: Many command-line options are accessible via the API (see `api.h`) when utilizing the library.

## ProcessController Program Command-Line Options

- **`-n <number of processes>`**:  
  Defines the number of child processes to create. Defaults to 4 if unspecified.  
  Example: `-n 5` to create 5 child processes.

- **`-t <process type>`**:  
  Specifies the type of process to create ('real', 'network', 'system', or 'simul'). Defaults to 'simul'.  
  Example: `-t real` to select real processes instead of simulated ones.

- **`-s <respawn>`**:  
  Toggles respawn functionality (0 for off, 1 for on).  
  Example: `-s 1` to enable respawn.

- **`-l <logging type>`**:  
  Sets the logging type ('console' or 'file'). Defaults to 'console'.  
  Example: `-l file` to output logs to a file.

- **`-T <nap time type>`**:  
  Sets the nap time type ('MS', 'SEC', 'MIN'). Defaults to 'MS'.  
  Example: `-T SEC` to use seconds as the nap time type.

- **`-c <configuration file path>`**:  
  Specifies the path to the configuration file.  
  Example: `-c /path/to/config/file` to use the designated configuration file.

- **`-h`**:  
  Displays usage information and exits the program.  
  Example: `-h` to view the help message with usage instructions.
## Building the Project

To build the project, you can use the provided `build.sh` script to clean, build, install, test, and debug the project. 
The script offers several options:
```bash
./build.sh
```
## Script Options

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
- **Execute Tests Debug**: Execute tests for the debug build.
- **Execute Tests Release**: Execute tests for the release build.
- **Execute Tests Debug-Coverage**: Execute tests for the debug-coverage build.
- **Execute Tests All**: Execute tests for all builds.
- **Debug ProcessController**: Debug the ProcessController.

**NOTE**: To use the script, simply run it and select the desired option from the menu. it can be executed by passing the desired option as an argument (type `./build.sh -h` for help).

## Building the Library and Executables

The project builds a static library and two executables:

- **Library**: `ProcessControllerLib`  
  Contains common source files used across the application.

- **Main Executable**: `ProcessController`  
  The main application executable.

- **Test Executable**: `ProcessControllerTests`  
  Contains test cases for various components of the application.

## Linking Libraries

The main executable and test executable link against the `ProcessControllerLib` library. The test executable also links 
against the Google Test libraries and pthread.

## Adding Google Test

Google Test is required for running the tests. Ensure it is installed and available on your system. The CMake 
configuration will find and include the necessary Google Test directories.

## Compiler Flags

The project uses the following compiler flags:
- `-Wall`: Enable all compilers warning messages.
- `-Wunused-variable`: Warn about unused variables.
- `-Wunused-function`: Warn about unused functions.

## Setting C++ Standard

The project is set to use the C++20 standard.

## Running the Program

Execute the program using the command below. This will run the executable located in the build directory:

```bash
./ProcessController -t system -n 2 # example
```

# Creating a Simple Program to Leverage the Library (prog_control)

This guide provides a walkthrough for setting up a new project, `prog_control`, that leverages the `ProcessControllerLib` library. In this example, you'll learn how to extend the `Process` and `ProcessMonitor` classes to create custom process and monitor implementations. It also shows you how to spawn external processes to monitor, specifically those requiring a configuration file.

Notably, the code for extending these classes is straightforward, demonstrating how easily you can build upon the base classes. By overriding or implementing the necessary methods, you can customize the behavior as needed. Otherwise, due to polymorphism, the derived classes will function like standard `Process` and `ProcessMonitor` classes, ensuring seamless integration and behavior consistency.

## Prerequisites

- **Compile the Process Controller Library**: Make sure the library is compiled by running the `build.sh` script in the `ProcessController` directory.
- **Install the Process Controller Library**: Ensure the library is installed by executing the same `build.sh` script in the `ProcessController` directory.
- **Create the Sample Program Directory Structure**: Set up the directory structure for the sample program.
- **Create the Sample Program CMake File**: Create a CMake file for the sample program.
- **Ensure your include and library paths are consistent with the installation directory**. See the CMake file below.
### 1. Create the Sample program Directory Structure

```sh
mkdir -p ~/dev/programs/testprogs/progcontrol
cd ~/dev/programs/testprogs/progcontrol
mkdir src include
```
### Sample program CMake file

Make sure that this sample program is linked against the proper library and includes the necessary headers. The CMake file should look like this:
(Dont link vs the code coverage build, if needed use:

```cmake
            # Enable code coverage flags
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
```


)
```cmake
cmake_minimum_required(VERSION 3.30)
project(ProgControl)

set(CMAKE_CXX_STANDARD 20)

# Enable code coverage flags if needed
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

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

### 2. Create the Sample program Main Program File

```cpp

#include "api/api.h"
#include "consolecontroller/console.h"

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

The most important part is the extension of Process and ProcessMonitor, and the initizlization of the controller with the new types.

```cpp

#include "process/process.h"
#include "processmonitors/process_monitor.h"
struct CustomMonitor final : process::ProcessMonitor
{
};

struct CustomProcess final : process::Process
{

};

...
    // Initialize controller with new type and number of processes
    api::registerHandler<CustomMonitor,CustomProcess>(processType);

```

### 3. Build the Sample Project

```sh

cd ~/dev/programs/testprogs/progcontrol
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target prog_control -j 6

 ```

### 4. Run the Sample Program

```sh
./prog_control  -t system -n 1 -c /Users/jrivero/dev/programs/ProcessController/exampleConfigFile.conf
```

## Examples of command line usage of the sample program

```sh
 ./prog_control -t system -n 2
 ./prog_control -l file
 ./prog_control -c path/to/ConfigFile.conf -t system -l file

```
### System process configuration file

The system process configuration file specifies the system process executable and its parameters. A sample configuration file might look like this:
```sh
process_file=../lengthy_process
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

### Appendix

#### Process Controller Key Components

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

#### Project Structure

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

#### Using the Arguments Class to Manage System Process Execution

We leverage the \`Arguments\` class to handle the configuration and
parameterization of the posix_spawn-created processes (system processes) , making it easier to handle the process' parameters.

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


