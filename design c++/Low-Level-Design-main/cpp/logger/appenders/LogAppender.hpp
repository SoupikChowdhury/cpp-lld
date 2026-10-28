#ifndef LOG_APPENDER_HPP
#define LOG_APPENDER_HPP

#include <string>
#include "../formatter/LogFormatter.hpp"
#include "../model/LogMessage.hpp"

class LogAppender {
protected:
    std::unique_ptr<LogFormatter> formatter;

public:
    explicit LogAppender(std::unique_ptr<LogFormatter> formatter) 
        : formatter(std::move(formatter)) {}
    virtual ~LogAppender() = default;

    virtual void append(const LogMessage& msg) = 0;
};

#endif // LOG_APPENDER_HPP
