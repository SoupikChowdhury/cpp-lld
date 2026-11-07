#ifndef CUSTOMER_SUPPORT_SERVICE_HPP
#define CUSTOMER_SUPPORT_SERVICE_HPP

#include <memory>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include "../repository/AgentRepository.hpp"
#include "../repository/IssueRepository.hpp"
#include "../strategy/AssignmentStrategy.hpp"
#include "../strategy/RoundRobinStrategy.hpp"

class CustomerSupportService {
private:
    AgentRepository agentRepo;
    IssueRepository issueRepo;
    std::unique_ptr<AssignmentStrategy> assignmentStrategy;
    mutable std::mutex mutex_;  // Makes choosing an agent and assigning to it one step

    void tryAssignFromWaitlist(Agent* agent) {
        if (issueRepo.hasWaitingIssues() && agent->isAvailable()) {
            // Prefer the oldest issue this agent has expertise in
            Issue* waitingIssue = issueRepo.getNextFromWaitlist([agent](const Issue* issue) {
                return agent->hasExpertise(issue->getType());
            });
            if (waitingIssue) {
                agent->assignIssue(waitingIssue);
                std::cout << "  [Auto-assigned] Issue " << waitingIssue->getIssueId()
                          << " from waitlist to Agent " << agent->getName() << std::endl;
            }
        }
    }

    bool tryAssign(Issue* issue) {
        std::vector<Agent*> agents = agentRepo.getAllAgents();
        Agent* agent = assignmentStrategy->assignIssue(agents, issue);
        if (agent == nullptr) {
            return false;
        }
        agent->assignIssue(issue);
        std::cout << "  [Assigned] Issue " << issue->getIssueId()
                  << " to Agent " << agent->getName() << std::endl;
        return true;
    }

    // For an issue taken back from an agent: it keeps its place ahead of newer issues
    void reassign(Issue* issue) {
        if (issue && !tryAssign(issue)) {
            issueRepo.addToFrontOfWaitlist(issue);
            std::cout << "  [Waitlist] Issue " << issue->getIssueId()
                      << " back at the front of the queue" << std::endl;
        }
    }

    Agent* requireAgent(const std::string& agentId) {
        Agent* agent = agentRepo.getAgent(agentId);
        if (agent == nullptr) {
            throw std::runtime_error("Agent not found: " + agentId);
        }
        return agent;
    }

public:
    CustomerSupportService() {
        assignmentStrategy = std::make_unique<RoundRobinStrategy>();
    }

    // Agent management
    void addAgent(Agent* agent) {
        std::lock_guard<std::mutex> lock(mutex_);
        agentRepo.addAgent(agent);
        tryAssignFromWaitlist(agentRepo.getAgent(agent->getAgentId()));
    }

    void removeAgent(const std::string& agentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Agent* agent = agentRepo.getAgent(agentId);
        if (agent) {
            Issue* issue = agent->releaseCurrentIssue();
            agentRepo.removeAgent(agentId);
            reassign(issue);
        }
    }

    Agent* getAgent(const std::string& agentId) {
        return agentRepo.getAgent(agentId);
    }

    void setAgentOnline(const std::string& agentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Agent* agent = requireAgent(agentId);
        agent->goOnline();
        tryAssignFromWaitlist(agent);
    }

    void setAgentOffline(const std::string& agentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Agent* agent = requireAgent(agentId);
        Issue* issue = agent->releaseCurrentIssue();
        agent->goOffline();
        reassign(issue);
    }

    // Strategy management
    void setAssignmentStrategy(std::unique_ptr<AssignmentStrategy> strategy) {
        std::lock_guard<std::mutex> lock(mutex_);
        assignmentStrategy = std::move(strategy);
    }

    std::string getActiveStrategy() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return assignmentStrategy->getStrategyName();
    }

    // Issue handling
    Issue* raiseIssue(const std::string& customerId, 
                      const std::string& description,
                      IssueType type) {
        std::lock_guard<std::mutex> lock(mutex_);

        // Create the issue
        Issue* issue = issueRepo.createIssue(customerId, description, type);

        // Try to assign to an agent
        if (!tryAssign(issue)) {
            issueRepo.addToWaitlist(issue);
            std::cout << "  [Waitlist] Issue " << issue->getIssueId() 
                      << " added to queue (no agents available)" << std::endl;
        }

        return issue;
    }

    void resolveIssue(const std::string& agentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Agent* agent = requireAgent(agentId);

        Issue* resolved = agent->resolveCurrentIssue();
        if (resolved) {
            std::cout << "  [Resolved] Issue " << resolved->getIssueId() 
                      << " by Agent " << agent->getName() << std::endl;
            
            // Try to assign next issue from waitlist
            tryAssignFromWaitlist(agent);
        }
    }

    void closeIssue(const std::string& issueId) {
        std::lock_guard<std::mutex> lock(mutex_);
        Issue* issue = issueRepo.getIssue(issueId);
        if (issue) {
            issue->close();
        }
    }

    // Status queries
    int getWaitlistSize() const {
        return issueRepo.getWaitlistSize();
    }

    int getAvailableAgentCount() const {
        return static_cast<int>(agentRepo.getAvailableAgents().size());
    }

    int getTotalAgentCount() const {
        return agentRepo.getAgentCount();
    }

    std::vector<Issue*> getAllIssues() const {
        return issueRepo.getAllIssues();
    }

    std::vector<Agent*> getAllAgents() {
        return agentRepo.getAllAgents();
    }

    void printStatus() const {
        std::cout << "\n--- Customer Support Status ---" << std::endl;
        std::cout << "Strategy: " << getActiveStrategy() << std::endl;
        std::cout << "Total Agents: " << getTotalAgentCount() << std::endl;
        std::cout << "Available Agents: " << getAvailableAgentCount() << std::endl;
        std::cout << "Waitlist Size: " << getWaitlistSize() << std::endl;
    }
};

#endif // CUSTOMER_SUPPORT_SERVICE_HPP
