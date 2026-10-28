#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <string>
#include "enums/LogLevel.hpp"
#include "model/LogMessage.hpp"
#include "handlers/LogHandler.hpp"
#include "LogHandlerConfiguration.hpp"

class Logger {
private:
    static inline Logger* instance = nullptr;
    LogHandler* handlerChain;

    Logger() {
        handlerChain = LogHandlerConfiguration::build();
    }

public:
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void log(LogLevel level, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        LogMessage msg(level, message, timestamp);
        handlerChain->handle(msg);
    }

    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    void info(const std::string& msg)  { log(LogLevel::INFO, msg); }
    void warn(const std::string& msg)  { log(LogLevel::WARN, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
    void fatal(const std::string& msg) { log(LogLevel::FATAL, msg); }
};

#endif // LOGGER_HPP
