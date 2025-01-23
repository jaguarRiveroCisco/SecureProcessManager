#pragma once

#ifndef TOOLS_NAP_TIME_H
#define TOOLS_NAP_TIME_H

namespace process
{
    enum NapTimeMs { SMALL = 50, SHORT = 100, MEDIUM = 500, LONG = 1000 };
    enum NapTimeSec { SHORTS = 5, MEDIUMS = 15, LONGS = 30 };
}

#endif // TOOLS_NAP_TIME_H