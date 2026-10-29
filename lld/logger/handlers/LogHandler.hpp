#ifndef LOG_HANDLER_HPP
#define LOG_HANDLER_HPP

#include <vector>
#include <memory>
#include "../enums/LogLevel.hpp"
#include "../model/LogMessage.hpp"
#include "../appenders/LogAppender.hpp"

class LogHandler {
protected:
    LogHandler* nextHandler = nullptr;
    std::vector<LogAppender*> appenders;

    virtual bool shouldHandle(LogLevel level) const = 0;

    void notifyAppenders(const LogMessage& msg) {
        for (auto* appender : appenders) {
            appender->append(msg);
        }
    }

public:
    virtual ~LogHandler() = default;

    void setNext(LogHandler* handler) {
        nextHandler = handler;
    }

    void addAppender(LogAppender* appender) {
        appenders.push_back(appender);
    }

    void handle(const LogMessage& msg) {
        if (shouldHandle(msg.getLevel())) {
            notifyAppenders(msg);
        }
        if (nextHandler) {
            nextHandler->handle(msg);
        }
    }
};

#endif // LOG_HANDLER_HPP
