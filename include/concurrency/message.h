#pragma once

#ifndef MESSAGE_H
#define MESSAGE_H

namespace concurrency
{
    static constexpr int MSG_SIZE = 100;

    struct Message
    {
        enum { CREATION_MSG = 1, TERMINATION_MSG };

        long             msgType      = CREATION_MSG;
        char             msgText[MSG_SIZE] {0};
    };

} // namespace concurrency
#endif // MESSAGE_H