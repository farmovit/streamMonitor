#include "Reader.h"
#include <iostream>
#include <sys/select.h>

void Reader::run() noexcept {
    try {
        while (!mIsInterrupted) {
            if (!stdinHasAvailableData()) {
                continue;
            }
            std::string str;
            std::getline(std::cin, str);
            if (str.empty()) {
                break;
            }
            dataAppeared(str);
        }
    } catch (std::exception &) {
        mLastException = std::current_exception();
    }
    destroyed();
}

std::exception_ptr Reader::lastException() const noexcept {
    return mLastException;
}

void Reader::interrupt() noexcept {
    mIsInterrupted = true;
}

bool Reader::stdinHasAvailableData() const {
    fd_set fds;
    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 200;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &time) > 0;
}
