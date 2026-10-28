#ifndef PLAIN_TEXT_FORMATTER_HPP
#define PLAIN_TEXT_FORMATTER_HPP

#include <sstream>
#include <iomanip>
#include <ctime>
#include "LogFormatter.hpp"

class PlainTextFormatter : public LogFormatter {
public:
    std::string format(const LogMessage& msg) const override {
        std::ostringstream oss;
        
        // Convert timestamp to readable format
        auto time = static_cast<std::time_t>(msg.getTimestamp() / 1000);
        std::tm* tm = std::localtime(&time);
        
        oss << "[" << std::put_time(tm, "%Y-%m-%d %H:%M:%S") << "] "
            << "[" << logLevelToString(msg.getLevel()) << "] "
            << msg.getMessage();
        
        return oss.str();
    }
};

#endif // PLAIN_TEXT_FORMATTER_HPP
