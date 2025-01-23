#pragma once

#ifndef TOOLS_RANDOM_STUFF_H
#define TOOLS_RANDOM_STUFF_H

namespace tools
{
    void sleepRandomMs(bool display = false);
    void sleepRandomSec(bool display = false);
    int  randomMs();
    int  randomSec();
    int  randomMin();
}

#endif // TOOLS_RANDOM_STUFF_H