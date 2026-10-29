#pragma once
#include "LogProcessor.hpp"

/**
 * Concrete handler for DEBUG level logs.
 * If log level is DEBUG, handles it; otherwise passes to next processor.
 */
class DebugLogProcessor : public LogProcessor {
public:
    explicit DebugLogProcessor(std::shared_ptr<LogProcessor> next = nullptr)
        : LogProcessor(next) {}

    void log(int logLevel, const std::string& message) override {
        if (logLevel == DEBUG) {
            std::cout << "DEBUG: " << message << std::endl;
        } else {
            LogProcessor::log(logLevel, message);
        }
    }
};
