#ifndef READER_H
#define READER_H

#include <boost/signals2.hpp>
#include <string>
#include <exception>

class Reader
{
public:
    Reader() = default;
    void runForever();
    boost::signals2::signal<void(const std::string &)> dataAppeared;
    boost::signals2::signal<void()> destroyed;
    std::exception_ptr lastException() const noexcept;
private:
    std::exception_ptr mLastException;
};

#endif // READER_H
