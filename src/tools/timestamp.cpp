#include "timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h> // For getpid()

std::string tools::TimeStamp::get()
{
    // Get current time with high precision
    auto now = std::chrono::system_clock::now();
    auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm    *now_tm   = std::localtime(&now_time);

    // Format the time with milliseconds
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count(); // Append milliseconds

    // Append process ID
    oss << " [PID: " << getpid() << "]";

    return oss.str();
}
