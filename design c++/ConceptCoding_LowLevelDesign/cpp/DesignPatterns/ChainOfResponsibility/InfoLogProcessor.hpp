#pragma once
#include "LogProcessor.hpp"

/**
 * Concrete handler for INFO level logs.
 * If log level is INFO, handles it; otherwise passes to next processor.
 */
class InfoLogProcessor : public LogProcessor {
public:
    explicit InfoLogProcessor(std::shared_ptr&lt;LogProcessor&gt; next = nullptr)
        : LogProcessor(next) {}

    void log(int logLevel, const std::string&amp; message) override {
        if (logLevel == INFO) {
            std::cout &lt;&lt; "INFO: " &lt;&lt; message &lt;&lt; std::endl;
        } else {
            LogProcessor::log(logLevel, message);
        }
    }
};
