#ifndef MESSAGE_H
#define MESSAGE_H

struct Message 
{
    long      msgType = MESSAGE_TYPE;
    char msgText[100];
    static const int MESSAGE_TYPE = 1; // Type for our messages
};

#endif // MESSAGE_H