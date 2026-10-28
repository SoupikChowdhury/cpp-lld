#pragma once
#include "LogProcessor.hpp"

/**
 * Concrete handler for INFO level logs.
 * If log level is INFO, handles it; otherwise passes to next processor.
 */
class InfoLogProcessor : public LogProcessor {
public:
    explicit InfoLogProcessor(std::shared_ptr<LogProcessor> next = nullptr)
        : LogProcessor(next) {}

    void log(int logLevel, const std::string& message) override {
        if (logLevel == INFO) {
            std::cout << "INFO: " << message << std::endl;
        } else {
            LogProcessor::log(logLevel, message);
        }
    }
};
