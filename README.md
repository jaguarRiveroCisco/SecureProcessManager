# My CMake Project

This project demonstrates the use of process creation in C++ using the `fork()` system call. It includes a simple program that creates a child process, prints messages indicating the role of each process, and waits for the child process to exit, reporting its exit status.

## Files

- `src/waitPidProc.cpp`: Contains the main function that implements the process creation and management logic.
- `CMakeLists.txt`: Configuration file for CMake that specifies the project settings and build instructions.

## Building the Project

To build the project, follow these steps:

1. Ensure you have CMake installed on your system.
2. Open a terminal and navigate to the project directory.
3. Configure:
   ```
   cmake -B build -S .
   ```
5. Build the project:
   ```
   cmake --build build --parallel 3
   ```

## Running the Program

After building the project, you can run the executable generated in the `build` directory:

```
build/waitPidProc [numprocs]
```
Defaults to 4 processes
This will execute the program, and you will see the output indicating the roles of the child and parent processes along with the exit status of the child process.