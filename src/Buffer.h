#ifndef BUFFER_H
#define BUFFER_H

#include <mutex>
#include <string>

class Buffer final
{
public:
    Buffer() = default;
    std::string popFront(int num);
    void appendBack(const std::string &str, char delimiter = ' ');
    std::string readAll() noexcept;
    bool hadData() const noexcept;
private:
    mutable std::mutex mReadWriteLock;
    std::string mBuffer;
    bool mHadData;
};

#endif // BUFFER_H
