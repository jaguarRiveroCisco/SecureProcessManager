#include "random_stuff.h"
#include "logger_instance.h"
#include <random>
#include <thread>
#include "nap_time.h"

namespace tools
{
    NapType SleepTime::type = NapType::MS;

    int randomMs()
    {
        // Initialize random number generator
        std::random_device rd;
        std::mt19937       gen(rd());

        // Sleep for a random duration in milliseconds
        std::uniform_int_distribution<> disMs(NapTimeMs::SMALL, NapTimeMs::LONG);
        return disMs(gen);
    }
    int randomSec()
    {
        // Initialize random number generator
        std::random_device rd;
        std::mt19937       gen(rd());

        // Sleep for a random duration in seconds
        std::uniform_int_distribution<> disSec(NapTimeSec::SMALLS, NapTimeSec::LONGS);
        return disSec(gen);
    }

    int randomMin()
    {
        // Initialize random number generator
        std::random_device rd;
        std::mt19937       gen(rd());

        // Sleep for a random duration in minutes
        std::uniform_int_distribution<> disMin(NapTimeMin::SMALLM, NapTimeMin::LONGM);
        return disMin(gen);
    }

    void sleepRandomMs(bool display)
    {
        auto nappy = randomMs();
        if (display)
        {
            tools::LoggerManager::getInstance().logInfo("Sleeping for: " + std::to_string(nappy) + " mS.");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(nappy));
    }

    void sleepRandomSec(bool display)
    {
        auto nappy = randomSec();
        if (display)
        {
            tools::LoggerManager::getInstance().logInfo("Sleeping for: " + std::to_string(nappy) + " s.");
        }
        std::this_thread::sleep_for(std::chrono::seconds(nappy));
    }
}