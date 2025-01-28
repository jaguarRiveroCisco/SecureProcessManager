#pragma once

#ifndef MESSAGE_H
#define MESSAGE_H

namespace concurrency
{
    struct Message
    {
        static const int CREATION_MSG = 1;
        long             msgType      = CREATION_MSG;
        char             msgText[100] {0};
    };
} // namespace concurrency
#endif // MESSAGE_H