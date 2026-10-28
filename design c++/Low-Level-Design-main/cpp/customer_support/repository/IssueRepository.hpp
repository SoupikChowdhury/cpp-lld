#ifndef ISSUE_REPOSITORY_HPP
#define ISSUE_REPOSITORY_HPP

#include <unordered_map>
#include <queue>
#include <mutex>
#include <sstream>
#include <iomanip>
#include "../model/Issue.hpp"

class IssueRepository {
private:
    std::unordered_map<std::string, Issue*> issues;
    std::queue<Issue*> waitlist;
    mutable std::mutex mutex_;
    int issueCounter = 0;

    std::string generateIssueId() {
        std::stringstream ss;
        ss << "ISS" << std::setfill('0') << std::setw(6) << (++issueCounter);
        return ss.str();
    }

public:
    ~IssueRepository() {
        for (auto& [id, issue] : issues) {
            delete issue;
        }
    }

    Issue* createIssue(const std::string& customerId, 
                       const std::string& description,
                       IssueType type) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string issueId = generateIssueId();
        Issue* issue = new Issue(issueId, customerId, description, type);
        issues[issueId] = issue;
        return issue;
    }

    Issue* getIssue(const std::string& issueId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = issues.find(issueId);
        return (it != issues.end()) ? it->second : nullptr;
    }

    void addToWaitlist(Issue* issue) {
        std::lock_guard<std::mutex> lock(mutex_);
        waitlist.push(issue);
    }

    Issue* getNextFromWaitlist() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (waitlist.empty()) return nullptr;
        Issue* next = waitlist.front();
        waitlist.pop();
        return next;
    }

    bool hasWaitingIssues() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return !waitlist.empty();
    }

    int getWaitlistSize() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<int>(waitlist.size());
    }

    std::vector<Issue*> getAllIssues() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Issue*> allIssues;
        for (const auto& [id, issue] : issues) {
            allIssues.push_back(issue);
        }
        return allIssues;
    }

    std::vector<Issue*> getOpenIssues() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Issue*> openIssues;
        for (const auto& [id, issue] : issues) {
            if (issue->getStatus() == IssueStatus::OPEN) {
                openIssues.push_back(issue);
            }
        }
        return openIssues;
    }
};

#endif // ISSUE_REPOSITORY_HPP
