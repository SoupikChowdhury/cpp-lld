#ifndef LOG_HANDLER_CONFIGURATION_HPP
#define LOG_HANDLER_CONFIGURATION_HPP

#include <unordered_map>
#include <memory>
#include "enums/LogLevel.hpp"
#include "handlers/LogHandler.hpp"
#include "handlers/DebugHandler.hpp"
#include "handlers/InfoHandler.hpp"
#include "handlers/WarnHandler.hpp"
#include "handlers/ErrorHandler.hpp"
#include "handlers/FatalHandler.hpp"
#include "appenders/LogAppender.hpp"

class LogHandlerConfiguration {
private:
    static inline std::unordered_map<LogLevel, LogHandler*> handlers;
    static inline LogHandler* debugHandler = nullptr;
    static inline LogHandler* infoHandler = nullptr;
    static inline LogHandler* warnHandler = nullptr;
    static inline LogHandler* errorHandler = nullptr;
    static inline LogHandler* fatalHandler = nullptr;

public:
    static LogHandler* build() {
        // Create handlers
        debugHandler = new DebugHandler();
        infoHandler = new InfoHandler();
        warnHandler = new WarnHandler();
        errorHandler = new ErrorHandler();
        fatalHandler = new FatalHandler();

        // Build chain
        debugHandler->setNext(infoHandler);
        infoHandler->setNext(warnHandler);
        warnHandler->setNext(errorHandler);
        errorHandler->setNext(fatalHandler);

        // Store in map for easy access
        handlers[LogLevel::DEBUG] = debugHandler;
        handlers[LogLevel::TRACE] = debugHandler;
        handlers[LogLevel::INFO] = infoHandler;
        handlers[LogLevel::WARN] = warnHandler;
        handlers[LogLevel::ERROR] = errorHandler;
        handlers[LogLevel::FATAL] = fatalHandler;

        return debugHandler;  // Return head of chain
    }

    static void addAppenderForLevel(LogLevel level, LogAppender* appender) {
        auto it = handlers.find(level);
        if (it != handlers.end()) {
            it->second->addAppender(appender);
        }
    }

    static LogHandler* getHandler(LogLevel level) {
        auto it = handlers.find(level);
        if (it != handlers.end()) {
            return it->second;
        }
        return nullptr;
    }
};

#endif // LOG_HANDLER_CONFIGURATION_HPP
