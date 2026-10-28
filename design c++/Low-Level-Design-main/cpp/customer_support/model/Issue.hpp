#ifndef ISSUE_HPP
#define ISSUE_HPP

#include <string>
#include <ctime>
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

    // Status transitions
    void setStatus(IssueStatus newStatus) { status = newStatus; }
    
    void assignToAgent(const std::string& agentId) {
        assignedAgentId = agentId;
        status = IssueStatus::IN_PROGRESS;
    }

    void resolve() {
        status = IssueStatus::RESOLVED;
        resolvedAt = time(nullptr);
    }

    void close() {
        status = IssueStatus::CLOSED;
    }

    bool isResolved() const {
        return status == IssueStatus::RESOLVED || status == IssueStatus::CLOSED;
    }
};

#endif // ISSUE_HPP
