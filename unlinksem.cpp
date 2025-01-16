/*
Compilation and usage
 g++ unlinksem.cpp -o unlinksem -Wall -std=c++17
./unlinksem /console_logger_semaphore

*/                                                                                                                     \

#include<semaphore.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <semaphore_name>" << std::endl;
        return 1;
    }

    const char *sem_name = argv[1];

    // Ensure the semaphore name starts with a slash
    if (sem_name[0] != '/') {
        std::cerr << "Semaphore name must start with a '/'." << std::endl;
        return 1;
    }

    // Unlink the semaphore
    if (sem_unlink(sem_name) == -1) {
        std::cerr << "Error unlinking semaphore " << sem_name << ": " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Semaphore " << sem_name << " unlinked successfully." << std::endl;
    return 0;
}
