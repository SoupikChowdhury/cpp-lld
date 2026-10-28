#pragma once
#include "LogProcessor.hpp"

/**
 * Concrete handler for ERROR level logs.
 * If log level is ERROR, handles it; otherwise passes to next processor.
 */
class ErrorLogProcessor : public LogProcessor {
public:
    explicit ErrorLogProcessor(std::shared_ptr&lt;LogProcessor&gt; next = nullptr)
        : LogProcessor(next) {}

    void log(int logLevel, const std::string&amp; message) override {
        if (logLevel == ERROR) {
            std::cout &lt;&lt; "ERROR: " &lt;&lt; message &lt;&lt; std::endl;
        } else {
            LogProcessor::log(logLevel, message);
        }
    }
};
