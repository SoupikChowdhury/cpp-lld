#ifndef JSON_FORMATTER_HPP
#define JSON_FORMATTER_HPP

#include <sstream>
#include "LogFormatter.hpp"

class JSONFormatter : public LogFormatter {
public:
    std::string format(const LogMessage& msg) const override {
        std::ostringstream oss;
        
        oss << "{"
            << "\"timestamp\":" << msg.getTimestamp() << ","
            << "\"level\":\"" << logLevelToString(msg.getLevel()) << "\","
            << "\"message\":\"" << escapeJson(msg.getMessage()) << "\""
            << "}";
        
        return oss.str();
    }

private:
    std::string escapeJson(const std::string& s) const {
        std::ostringstream o;
        for (char c : s) {
            switch (c) {
                case '"': o << "\\\""; break;
                case '\\': o << "\\\\"; break;
                case '\b': o << "\\b"; break;
                case '\f': o << "\\f"; break;
                case '\n': o << "\\n"; break;
                case '\r': o << "\\r"; break;
                case '\t': o << "\\t"; break;
                default: o << c;
            }
        }
        return o.str();
    }
};

#endif // JSON_FORMATTER_HPP
