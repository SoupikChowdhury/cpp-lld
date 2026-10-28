#pragma once
#include "LogProcessor.hpp"

/**
 * Concrete handler for DEBUG level logs.
 * If log level is DEBUG, handles it; otherwise passes to next processor.
 */
class DebugLogProcessor : public LogProcessor {
public:
    explicit DebugLogProcessor(std::shared_ptr&lt;LogProcessor&gt; next = nullptr)
        : LogProcessor(next) {}

    void log(int logLevel, const std::string&amp; message) override {
        if (logLevel == DEBUG) {
            std::cout &lt;&lt; "DEBUG: " &lt;&lt; message &lt;&lt; std::endl;
        } else {
            LogProcessor::log(logLevel, message);
        }
    }
};
