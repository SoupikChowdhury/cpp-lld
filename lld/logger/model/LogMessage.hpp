#ifndef LOG_MESSAGE_HPP
#define LOG_MESSAGE_HPP

#include <string>
#include <chrono>
#include "../enums/LogLevel.hpp"

class LogMessage {
private:
    LogLevel level;
    std::string message;
    long long timestamp;

public:
    LogMessage(LogLevel level, const std::string& message, long long timestamp)
        : level(level), message(message), timestamp(timestamp) {}

    LogLevel getLevel() const { return level; }
    std::string getMessage() const { return message; }
    long long getTimestamp() const { return timestamp; }
};

#endif // LOG_MESSAGE_HPP
