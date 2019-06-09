#ifndef READER_H
#define READER_H

#include <boost/signals2.hpp>
#include <string>
#include <exception>

class Reader final
{
public:
    Reader() = default;
    void run() noexcept;
    boost::signals2::signal<void(const std::string &)> dataAppeared;
    boost::signals2::signal<void()> destroyed;
    std::exception_ptr lastException() const noexcept;
    void interrupt() noexcept;
private:
    bool stdinHasAvailableData() const;
private:
    std::atomic_bool mIsInterrupted{false};
    std::exception_ptr mLastException{nullptr};
};

#endif // READER_H
