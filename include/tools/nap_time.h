#pragma once

#ifndef TOOLS_NAP_TIME_H
#define TOOLS_NAP_TIME_H

namespace tools
{
    enum NapTimeMs { SMALL = 50, SHORT = 100, MEDIUM = 500, LONG = 1000 };
    enum NapTimeSec { SMALLS = 1, SHORTS = 5, MEDIUMS = 15, LONGS = 30 };
    enum NapTimeMin { SMALLM = 1, SHORTM = 5, MEDIUMM = 10, LONGM = 30 };
}

#endif // TOOLS_NAP_TIME_H