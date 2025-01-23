#pragma once

#ifndef TOOLS_NAP_TIME_H
#define TOOLS_NAP_TIME_H

namespace tools
{
    enum NapTimeMs { SMALL = 50, SHORT = 100, MEDIUM = 500, LONG = 1000 };
    enum NapTimeSec { SMALLS = 1, SHORTS = 5, MEDIUMS = 15, LONGS = 30 };
    enum NapTimeMin { SMALLM = 1, SHORTM = 5, MEDIUMM = 10, LONGM = 30 };
    enum NapType { MS, SEC, MIN };
    struct SleepTime 
    {
        static NapType type;
        static const char* NapTypeToString() 
        {
            switch (type) {
            case MS: return "Milliseconds";
            case SEC: return "Seconds";
            case MIN: return "Minutes";
            default: return "Unknown";
            }
        }
    };
} // namespace tools

#endif // TOOLS_NAP_TIME_H