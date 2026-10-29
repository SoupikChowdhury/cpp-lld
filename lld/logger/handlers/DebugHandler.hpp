#ifndef DEBUG_HANDLER_HPP
#define DEBUG_HANDLER_HPP

#include "LogHandler.hpp"

class DebugHandler : public LogHandler {
protected:
    bool shouldHandle(LogLevel level) const override {
        return level == LogLevel::DEBUG || level == LogLevel::TRACE;
    }
};

#endif // DEBUG_HANDLER_HPP
