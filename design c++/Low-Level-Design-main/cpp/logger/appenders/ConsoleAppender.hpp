#ifndef CONSOLE_APPENDER_HPP
#define CONSOLE_APPENDER_HPP

#include <iostream>
#include "LogAppender.hpp"

class ConsoleAppender : public LogAppender {
public:
    explicit ConsoleAppender(std::unique_ptr<LogFormatter> formatter)
        : LogAppender(std::move(formatter)) {}

    void append(const LogMessage& msg) override {
        std::cout << formatter->format(msg) << std::endl;
    }
};

#endif // CONSOLE_APPENDER_HPP
