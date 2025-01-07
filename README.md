# Process Management Program

## Overview

The Process Management Program is a C++ application designed to handle and monitor multiple child processes. It leverages threading, inter-process communication (IPC), and synchronization to efficiently manage process creation and lifecycle events.

## Key Components

- **Messenger**: Facilitates message passing between processes for communication.
- **Synchro**: Utilizes mutexes and condition variables to synchronize process events.
- **ProcessBase**: Provides core functionalities for checking process status and handling termination.
- **SimulProcess**: Simulates workload for child processes with randomized sleep durations.
- **ProcessHelper**: Offers utilities for messaging and other process management tasks.
- **ProcessHandler**: Integrates the functionalities of all components to create, monitor, and manage child processes.

## Features

- **Process Creation**: Dynamically spawns a specified number of child processes.
- **Threaded Monitoring**: Each child process is monitored in its own thread to track its status and handle termination effectively.
- **IPC and Synchronization**: Ensures smooth communication and synchronization between processes using Messenger and Synchro.
- **Event Handling**: The main thread waits for and processes events as child processes exit.

## Files

- **`src/waitPidProc.cpp`**: Implements the main process creation and management logic.
- **`CMakeLists.txt`**: Configuration file for CMake, specifying project settings and build instructions.

## Building the Project

To build the project, perform the following steps:

1. **Install CMake**: Ensure CMake is installed on your system.
2. **Navigate to Project Directory**: Open a terminal and go to the project root directory.
3. **Configure the Project**:
   ```bash
   cmake -B build -S .
   ```
4. **Build the Project**:
   ```bash
   cmake --build build --parallel 3
   ```

## Running the Program

Once the project is built, execute the program using the command below. This will run the executable located in the `build` directory:

```bash
build/waitPidProc [numprocs]
```

- **`[numprocs]`**: Optional parameter to specify the number of child processes to create. Defaults to 4 if not provided.

Upon execution, the program will display output detailing the roles of the child and parent processes, along with the exit status of each child process.