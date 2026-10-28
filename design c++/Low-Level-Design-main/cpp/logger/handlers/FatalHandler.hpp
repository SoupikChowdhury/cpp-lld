#ifndef FATAL_HANDLER_HPP
#define FATAL_HANDLER_HPP

#include "LogHandler.hpp"

class FatalHandler : public LogHandler {
protected:
    bool shouldHandle(LogLevel level) const override {
        return level == LogLevel::FATAL;
    }
};

#endif // FATAL_HANDLER_HPP
