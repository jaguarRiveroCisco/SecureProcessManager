#include "timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h> // For getpid()

std::string tools::TimeStamp::get() 
{
    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Format the time
    std::tm* now_tm = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y%m%d_%H%M%S"); // Format: YYYYMMDD_HHMMSS

    // Append process ID
    oss << "_PID" << getpid();

    return oss.str();
}
