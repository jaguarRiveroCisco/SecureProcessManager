#ifndef MESSAGE_H
#define MESSAGE_H

struct Message 
{
    long      msgType = CREATION_MSG;
    char msgText[100];
    static const int CREATION_MSG = 1; // Type for our messages
};

#endif // MESSAGE_H