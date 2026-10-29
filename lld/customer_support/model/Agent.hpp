#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "Issue.hpp"
#include "../enums/AgentStatus.hpp"
#include "../enums/IssueType.hpp"

class Agent {
private:
    std::string agentId;
    std::string name;
    AgentStatus status;
    std::vector<IssueType> expertise;
    Issue* currentIssue;
    int resolvedCount;

public:
    Agent(const std::string& agentId, const std::string& name,
          const std::vector<IssueType>& expertise = {})
        : agentId(agentId), name(name), status(AgentStatus::AVAILABLE),
          expertise(expertise), currentIssue(nullptr), resolvedCount(0) {}

    // Getters
    std::string getAgentId() const { return agentId; }
    std::string getName() const { return name; }
    AgentStatus getStatus() const { return status; }
    Issue* getCurrentIssue() const { return currentIssue; }
    int getResolvedCount() const { return resolvedCount; }
    const std::vector<IssueType>& getExpertise() const { return expertise; }

    bool isAvailable() const { 
        return status == AgentStatus::AVAILABLE && currentIssue == nullptr; 
    }

    bool hasExpertise(IssueType issueType) const {
        if (expertise.empty()) return true;  // Can handle all if no specific expertise
        return std::find(expertise.begin(), expertise.end(), issueType) != expertise.end();
    }

    void addExpertise(IssueType type) {
        if (!hasExpertise(type)) {
            expertise.push_back(type);
        }
    }

    void assignIssue(Issue* issue) {
        currentIssue = issue;
        status = AgentStatus::BUSY;
        issue->assignToAgent(agentId);
    }

    Issue* resolveCurrentIssue() {
        if (currentIssue == nullptr) return nullptr;
        
        currentIssue->resolve();
        Issue* resolved = currentIssue;
        currentIssue = nullptr;
        status = AgentStatus::AVAILABLE;
        resolvedCount++;
        
        return resolved;
    }

    void goOnline() { 
        if (status == AgentStatus::OFFLINE) {
            status = AgentStatus::AVAILABLE;
        }
    }

    void goOffline() { 
        status = AgentStatus::OFFLINE; 
    }

    std::string getExpertiseString() const {
        if (expertise.empty()) return "ALL";
        std::string result;
        for (size_t i = 0; i < expertise.size(); i++) {
            result += issueTypeToString(expertise[i]);
            if (i < expertise.size() - 1) result += ", ";
        }
        return result;
    }
};

#endif // AGENT_HPP
