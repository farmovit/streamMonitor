#include "Buffer.h"

std::string Buffer::popFront(int num) {
    if (num <= 0) {
        return {};
    }
    std::lock_guard lock(mReadWriteLock);
    if (mBuffer.size() <= static_cast<std::size_t>(num)) {
        return std::move(mBuffer);
    }
    auto result = mBuffer.substr(0, static_cast<std::size_t>(num - 1));
    mBuffer.erase(0, static_cast<std::size_t>(num - 1));
    return result;
}

void Buffer::appendBack(const std::string &str, char delimiter) {
    std::lock_guard lock(mReadWriteLock);
    mBuffer.append(str + delimiter);
}

std::string Buffer::readAll() noexcept {
    std::lock_guard lock(mReadWriteLock);
    return std::move(mBuffer);
}
