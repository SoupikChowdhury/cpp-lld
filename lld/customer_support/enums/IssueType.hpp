#ifndef ISSUE_TYPE_HPP
#define ISSUE_TYPE_HPP

#include <string>

enum class IssueType {
    BILLING,
    TECHNICAL,
    GENERAL,
    ACCOUNT
};

inline std::string issueTypeToString(IssueType type) {
    switch (type) {
        case IssueType::BILLING: return "BILLING";
        case IssueType::TECHNICAL: return "TECHNICAL";
        case IssueType::GENERAL: return "GENERAL";
        case IssueType::ACCOUNT: return "ACCOUNT";
        default: return "UNKNOWN";
    }
}

#endif // ISSUE_TYPE_HPP
