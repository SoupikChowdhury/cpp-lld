#pragma once
#include "LogProcessor.hpp"

/**
 * Concrete handler for ERROR level logs.
 * If log level is ERROR, handles it; otherwise passes to next processor.
 */
class ErrorLogProcessor : public LogProcessor {
public:
    explicit ErrorLogProcessor(std::shared_ptr<LogProcessor> next = nullptr)
        : LogProcessor(next) {}

    void log(int logLevel, const std::string& message) override {
        if (logLevel == ERROR) {
            std::cout << "ERROR: " << message << std::endl;
        } else {
            LogProcessor::log(logLevel, message);
        }
    }
};
