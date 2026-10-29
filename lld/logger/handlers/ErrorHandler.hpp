#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include "LogHandler.hpp"

class ErrorHandler : public LogHandler {
protected:
    bool shouldHandle(LogLevel level) const override {
        return level == LogLevel::ERROR;
    }
};

#endif // ERROR_HANDLER_HPP
