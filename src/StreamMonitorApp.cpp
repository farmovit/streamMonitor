#include "StreamMonitorApp.h"
#include <iostream>
#include <string>
#include <future>
#include <csignal>
#include "Buffer.h"
#include "Reader.h"

using namespace std::literals;

namespace {
std::atomic_bool isInterrupted{false};
constexpr auto writeTimeout{1s};
constexpr int writeCharsNum{80};
} // namespace

StreamMonitorApp::StreamMonitorApp() {
    initialize();
}

void StreamMonitorApp::initialize() {
    signal(SIGINT, []([[ maybe_unused ]] int sig) { isInterrupted = true; });
    signal(SIGTERM, []([[ maybe_unused ]] int sig) { isInterrupted = true; });
    mReader.dataAppeared.connect([this](const std::string &str) {
        try {
            mBuffer.appendBack(str);
        } catch (std::exception &) {
            addException(std::current_exception());
        }
    });
    mReader.destroyed.connect([]{ isInterrupted = true; });
}

int StreamMonitorApp::exec() noexcept {
    try {
        auto writerFuture = std::async(std::launch::async, [this] { runWriter(); });
        auto readerFuture = std::async(std::launch::async, [this] { mReader.run(); });
        writerFuture.wait();
        mReader.interrupt();
        readerFuture.wait();
    } catch (const std::exception&) {
        addException(std::current_exception());
    }
    if (auto exception = mReader.lastException();
            exception != nullptr) {
        addException(exception);
    }
    if (mExceptions.empty()) {
        return 0;
    }
    for (const auto &exception: mExceptions) {
        try {
           std::rethrow_exception(exception);
        } catch (const std::exception& e) {
           std::cerr << "An error occured during the program execution: " << e.what() << std::endl;
        }
    }
    return 1;
}

void StreamMonitorApp::runWriter() noexcept {
    while (!isInterrupted) {
        std::this_thread::sleep_for(writeTimeout);
        try {
            auto str = mBuffer.popFront(writeCharsNum);
            if (!str.empty()) {
                std::cout << str << std::endl;
            }
        } catch (std::exception &) {
            addException(std::current_exception());
        }
    }
    auto str = mBuffer.readAll();
    if (!str.empty()) {
        std::cout << str << std::endl;
    }
}

void StreamMonitorApp::addException(const std::exception_ptr &exception) {
    isInterrupted = true;
    std::lock_guard lock(mReadWriteLocker);
    mExceptions.emplace_back(exception);
}
