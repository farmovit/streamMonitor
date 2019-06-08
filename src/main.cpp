#include <iostream>
#include <string>
#include <future>
#include "Buffer.h"
#include "Reader.h"
#include <csignal>

using namespace std::literals;

namespace {
std::atomic_bool isInterrupted{false};
constexpr auto writeTimeout{1s};
constexpr int writeCharsNum{80};
} // namespace

int main() {
    signal(SIGINT, []([[ maybe_unused ]] int sig) { isInterrupted = true; });
    signal(SIGTERM, []([[ maybe_unused ]] int sig) { isInterrupted = true; });

    Buffer buffer;
    Reader reader;
    reader.dataAppeared.connect([&buffer](const std::string &str) {
        buffer.appendBack(str);
    });
    reader.destroyed.connect([]{
        isInterrupted = true;
    });

    auto dummy = std::async(std::launch::async, [&] {
        while (!isInterrupted) {
            std::this_thread::sleep_for(writeTimeout);
            auto str = buffer.popFront(writeCharsNum);
            if (!str.empty()) {
                std::cout << "ENABLED OUTPUT\n";
                std::cout << str << std::endl;
            }
        }
        auto str = buffer.readAll();
        if (!str.empty()) {
            std::cout << "FINAL OUTPUT\n";
            std::cout << str << std::endl;
        }
//        std::exit(0);
//        std::cin.clear();
//        std::cin.ignore();
    });

    auto readerFuture = std::async(std::launch::async, [&reader] {
       reader.runForever();
    });

    dummy.wait();

    std::cout << "EXIT! " << std::endl;

    return 0;
}
