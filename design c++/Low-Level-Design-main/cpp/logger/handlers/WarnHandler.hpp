#ifndef WARN_HANDLER_HPP
#define WARN_HANDLER_HPP

#include "LogHandler.hpp"

class WarnHandler : public LogHandler {
protected:
    bool shouldHandle(LogLevel level) const override {
        return level == LogLevel::WARN;
    }
};

#endif // WARN_HANDLER_HPP
