#ifndef ISSUE_STATUS_HPP
#define ISSUE_STATUS_HPP

#include <string>

enum class IssueStatus {
    OPEN,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
};

inline std::string issueStatusToString(IssueStatus status) {
    switch (status) {
        case IssueStatus::OPEN: return "OPEN";
        case IssueStatus::IN_PROGRESS: return "IN_PROGRESS";
        case IssueStatus::RESOLVED: return "RESOLVED";
        case IssueStatus::CLOSED: return "CLOSED";
        default: return "UNKNOWN";
    }
}

#endif // ISSUE_STATUS_HPP
