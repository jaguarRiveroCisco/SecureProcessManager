#ifndef MESSAGE_H
#define MESSAGE_H

struct Message 
{
    long      msgType = CREATION_MSG;
    char msgText[100] {0};
    static const int CREATION_MSG = 1;
};

#endif // MESSAGE_H