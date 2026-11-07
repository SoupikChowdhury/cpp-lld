#ifndef ISSUE_HPP
#define ISSUE_HPP

#include <string>
#include <ctime>
#include <stdexcept>
#include "../enums/IssueType.hpp"
#include "../enums/IssueStatus.hpp"

class Issue {
private:
    std::string issueId;
    std::string customerId;
    std::string description;
    IssueType type;
    IssueStatus status;
    time_t createdAt;
    time_t resolvedAt;
    std::string assignedAgentId;

    void requireStatus(IssueStatus required, const char* action) const {
        if (status != required) {
            throw std::runtime_error(std::string("Cannot ") + action + " issue " + issueId +
                                     ": it is " + issueStatusToString(status));
        }
    }

public:
    Issue(const std::string& issueId, const std::string& customerId,
          const std::string& description, IssueType type)
        : issueId(issueId), customerId(customerId), description(description),
          type(type), status(IssueStatus::OPEN), resolvedAt(0) {
        createdAt = time(nullptr);
    }

    // Getters
    std::string getIssueId() const { return issueId; }
    std::string getCustomerId() const { return customerId; }
    std::string getDescription() const { return description; }
    IssueType getType() const { return type; }
    IssueStatus getStatus() const { return status; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getResolvedAt() const { return resolvedAt; }
    std::string getAssignedAgentId() const { return assignedAgentId; }

    // Status transitions: each only from the status that allows it
    void assignToAgent(const std::string& agentId) {
        requireStatus(IssueStatus::OPEN, "assign");
        assignedAgentId = agentId;
        status = IssueStatus::IN_PROGRESS;
    }

    void releaseFromAgent() {
        requireStatus(IssueStatus::IN_PROGRESS, "release");
        assignedAgentId.clear();
        status = IssueStatus::OPEN;
    }

    void resolve() {
        requireStatus(IssueStatus::IN_PROGRESS, "resolve");
        status = IssueStatus::RESOLVED;
        resolvedAt = time(nullptr);
    }

    void close() {
        requireStatus(IssueStatus::RESOLVED, "close");
        status = IssueStatus::CLOSED;
    }

    bool isResolved() const {
        return status == IssueStatus::RESOLVED || status == IssueStatus::CLOSED;
    }
};

#endif // ISSUE_HPP
