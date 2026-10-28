#include "InfoLogProcessor.hpp"
#include "DebugLogProcessor.hpp"
#include "ErrorLogProcessor.hpp"

/**
 * Chain of Responsibility Pattern Demo
 * 
 * Creates a chain: InfoLogProcessor -> DebugLogProcessor -> ErrorLogProcessor
 * Each log request traverses the chain until it finds the right handler.
 */
int main() {
    // Build the chain from end to beginning
    auto errorProcessor = std::make_shared&lt;ErrorLogProcessor&gt;(nullptr);
    auto debugProcessor = std::make_shared&lt;DebugLogProcessor&gt;(errorProcessor);
    auto infoProcessor = std::make_shared&lt;InfoLogProcessor&gt;(debugProcessor);

    // Test the chain with different log levels
    infoProcessor-&gt;log(LogProcessor::ERROR, "exception happens");
    infoProcessor-&gt;log(LogProcessor::DEBUG, "need to debug this");
    infoProcessor-&gt;log(LogProcessor::INFO, "just for info");

    return 0;
}
