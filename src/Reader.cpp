#include "Reader.h"
#include <iostream>

void Reader::runForever() {
    try {
        std::string str;
        while (std::getline(std::cin, str)) {
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
