#pragma once
#include <iostream>
#include <string>
#include <memory>

/**
 * Chain of Responsibility Design Pattern
 * 
 * Abstract base class for log processors.
 * Each processor handles specific log levels and passes
 * unhandled requests to the next processor in the chain.
 */
class LogProcessor {
public:
    static const int INFO = 1;
    static const int DEBUG = 2;
    static const int ERROR = 3;

protected:
    std::shared_ptr<LogProcessor> nextLogProcessor;

public:
    explicit LogProcessor(std::shared_ptr<LogProcessor> next = nullptr)
        : nextLogProcessor(next) {}

    virtual ~LogProcessor() = default;

    virtual void log(int logLevel, const std::string& message) {
        if (nextLogProcessor != nullptr) {
            nextLogProcessor->log(logLevel, message);
        }
    }
};
