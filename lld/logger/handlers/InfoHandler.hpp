#ifndef INFO_HANDLER_HPP
#define INFO_HANDLER_HPP

#include "LogHandler.hpp"

class InfoHandler : public LogHandler {
protected:
    bool shouldHandle(LogLevel level) const override {
        return level == LogLevel::INFO;
    }
};

#endif // INFO_HANDLER_HPP
