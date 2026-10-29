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
    auto errorProcessor = std::make_shared<ErrorLogProcessor>(nullptr);
    auto debugProcessor = std::make_shared<DebugLogProcessor>(errorProcessor);
    auto infoProcessor = std::make_shared<InfoLogProcessor>(debugProcessor);

    // Test the chain with different log levels
    infoProcessor->log(LogProcessor::ERROR, "exception happens");
    infoProcessor->log(LogProcessor::DEBUG, "need to debug this");
    infoProcessor->log(LogProcessor::INFO, "just for info");

    return 0;
}
