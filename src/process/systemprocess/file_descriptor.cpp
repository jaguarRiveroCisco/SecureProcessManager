#include <unistd.h>
#include <stdexcept>
#include <string>
#include "file_descriptor.h"
#include "system_process.h"
#include "communicator.h"
#include "process_status.h"
#include "process_controller.h"
#include "string_tools.h"
#include <sys/fcntl.h>

FileDescriptor::FileDescriptor(const std::string &fileName)
{
    fd_ = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_ == -1) {
        throw std::runtime_error("Failed to open output file: " + fileName);
    }
}

FileDescriptor::~FileDescriptor()
{
    if (fd_ != -1) {
        close(fd_);
    }
}

int FileDescriptor::get() const
{
    return fd_;
}
