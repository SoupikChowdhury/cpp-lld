// Logger System - Main Entry Point
// Demonstrates Singleton, Chain of Responsibility, Observer, and Strategy patterns

#include <iostream>
#include <memory>
#include "Logger.hpp"
#include "LogHandlerConfiguration.hpp"
#include "appenders/ConsoleAppender.hpp"
#include "appenders/FileAppender.hpp"
#include "formatter/PlainTextFormatter.hpp"
#include "formatter/JSONFormatter.hpp"

int main() {
    std::cout << "=== Logger System Demo ===" << std::endl;
    std::cout << std::endl;

    // Get singleton logger instance
    Logger* logger = Logger::getInstance();

    // Configure appenders for different log levels
    
    // INFO level -> Console with PlainText
    auto infoConsoleAppender = new ConsoleAppender(
        std::make_unique<PlainTextFormatter>()
    );
    LogHandlerConfiguration::addAppenderForLevel(LogLevel::INFO, infoConsoleAppender);

    // ERROR level -> Console with PlainText + File with JSON
    auto errorConsoleAppender = new ConsoleAppender(
        std::make_unique<PlainTextFormatter>()
    );
    auto errorFileAppender = new FileAppender(
        std::make_unique<JSONFormatter>(),
        "error_logs.json"
    );
    LogHandlerConfiguration::addAppenderForLevel(LogLevel::ERROR, errorConsoleAppender);
    LogHandlerConfiguration::addAppenderForLevel(LogLevel::ERROR, errorFileAppender);

    // FATAL level -> Console + File
    auto fatalConsoleAppender = new ConsoleAppender(
        std::make_unique<PlainTextFormatter>()
    );
    LogHandlerConfiguration::addAppenderForLevel(LogLevel::FATAL, fatalConsoleAppender);

    // DEBUG level -> Console
    auto debugConsoleAppender = new ConsoleAppender(
        std::make_unique<PlainTextFormatter>()
    );
    LogHandlerConfiguration::addAppenderForLevel(LogLevel::DEBUG, debugConsoleAppender);

    std::cout << "--- Logging Messages ---" << std::endl;
    std::cout << std::endl;

    // Log messages at different levels
    logger->debug("This is a debug message");
    logger->info("This is some key information");
    logger->warn("This is a warning");  // No appender configured, won't output
    logger->error("Oh no! there's an error");
    logger->fatal("Critical system failure!");

    std::cout << std::endl;
    std::cout << "Check 'error_logs.json' for JSON formatted error logs." << std::endl;

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    // Cleanup (in production, use smart pointers properly)
    delete infoConsoleAppender;
    delete errorConsoleAppender;
    delete errorFileAppender;
    delete fatalConsoleAppender;
    delete debugConsoleAppender;

    return 0;
}
