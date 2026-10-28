#ifndef LOG_FORMATTER_HPP
#define LOG_FORMATTER_HPP

#include <string>
#include "../model/LogMessage.hpp"

class LogFormatter {
public:
    virtual ~LogFormatter() = default;
    virtual std::string format(const LogMessage& msg) const = 0;
};

#endif // LOG_FORMATTER_HPP
