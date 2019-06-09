#ifndef STREAMMONITORAPP_H
#define STREAMMONITORAPP_H

#include <exception>
#include <vector>
#include <mutex>
#include "Buffer.h"
#include "Reader.h"

class StreamMonitorApp final
{
public:
    StreamMonitorApp();
    int exec() noexcept;
private:
    void initialize();
    void runWriter() noexcept;
    void addException(const std::exception_ptr &exception);
private:
    Buffer mBuffer;
    Reader mReader;
    std::vector<std::exception_ptr> mExceptions;
    mutable std::mutex mReadWriteLocker;
};

#endif // STREAMMONITORAPP_H
