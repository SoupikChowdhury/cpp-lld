#ifndef FILE_APPENDER_HPP
#define FILE_APPENDER_HPP

#include <fstream>
#include <string>
#include "LogAppender.hpp"

class FileAppender : public LogAppender {
private:
    std::string filePath;

public:
    FileAppender(std::unique_ptr<LogFormatter> formatter, const std::string& filePath)
        : LogAppender(std::move(formatter)), filePath(filePath) {}

    void append(const LogMessage& msg) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << formatter->format(msg) << std::endl;
            file.close();
        }
    }
};

#endif // FILE_APPENDER_HPP
