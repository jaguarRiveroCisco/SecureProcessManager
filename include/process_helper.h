#ifndef PROCESS_HELPER_H
#define PROCESS_HELPER_H
class ProcessHelper 
{
public:
    std::string receiveMessage() { return messenger_.receiveMessage(); }

protected:
    ProcessHelper() = default;
    void sendMessage(int sleepDuration, pid_t pid = getpid())
    {
        std::string messageText = "Child " + std::to_string(pid) +
                                  " created. Sleep duration: " + std::to_string(sleepDuration) + " seconds";
        try
        {
            messenger_.sendMessage(messageText);
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Failed to send message: " << e.what() << std::endl;
        }
    }
    static Messenger messenger_;
};

#endif // PROCESS_HELPER_H
