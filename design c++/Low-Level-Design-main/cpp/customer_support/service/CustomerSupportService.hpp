#ifndef CUSTOMER_SUPPORT_SERVICE_HPP
#define CUSTOMER_SUPPORT_SERVICE_HPP

#include <memory>
#include <iostream>
#include "../repository/AgentRepository.hpp"
#include "../repository/IssueRepository.hpp"
#include "../strategy/AssignmentStrategy.hpp"
#include "../strategy/RoundRobinStrategy.hpp"

class CustomerSupportService {
private:
    AgentRepository agentRepo;
    IssueRepository issueRepo;
    std::unique_ptr<AssignmentStrategy> assignmentStrategy;

    void tryAssignFromWaitlist(Agent* agent) {
        if (issueRepo.hasWaitingIssues() && agent->isAvailable()) {
            Issue* waitingIssue = issueRepo.getNextFromWaitlist();
            if (waitingIssue) {
                agent->assignIssue(waitingIssue);
                std::cout << "  [Auto-assigned] Issue " << waitingIssue->getIssueId() 
                          << " from waitlist to Agent " << agent->getName() << std::endl;
            }
        }
    }

public:
    CustomerSupportService() {
        assignmentStrategy = std::make_unique<RoundRobinStrategy>();
    }

    // Agent management
    void addAgent(Agent* agent) {
        agentRepo.addAgent(agent);
    }

    void removeAgent(const std::string& agentId) {
        agentRepo.removeAgent(agentId);
    }

    Agent* getAgent(const std::string& agentId) {
        return agentRepo.getAgent(agentId);
    }

    // Strategy management
    void setAssignmentStrategy(std::unique_ptr<AssignmentStrategy> strategy) {
        assignmentStrategy = std::move(strategy);
    }

    std::string getActiveStrategy() const {
        return assignmentStrategy->getStrategyName();
    }

    // Issue handling
    Issue* raiseIssue(const std::string& customerId, 
                      const std::string& description,
                      IssueType type) {
        // Create the issue
        Issue* issue = issueRepo.createIssue(customerId, description, type);

        // Try to assign to an agent
        Agent* agent = assignmentStrategy->assignIssue(agentRepo.getAllAgents(), issue);
        
        if (agent != nullptr) {
            agent->assignIssue(issue);
            std::cout << "  [Assigned] Issue " << issue->getIssueId() 
                      << " to Agent " << agent->getName() << std::endl;
        } else {
            issueRepo.addToWaitlist(issue);
            std::cout << "  [Waitlist] Issue " << issue->getIssueId() 
                      << " added to queue (no agents available)" << std::endl;
        }

        return issue;
    }

    void resolveIssue(const std::string& agentId) {
        Agent* agent = agentRepo.getAgent(agentId);
        if (agent == nullptr) {
            throw std::runtime_error("Agent not found: " + agentId);
        }

        Issue* resolved = agent->resolveCurrentIssue();
        if (resolved) {
            std::cout << "  [Resolved] Issue " << resolved->getIssueId() 
                      << " by Agent " << agent->getName() << std::endl;
            
            // Try to assign next issue from waitlist
            tryAssignFromWaitlist(agent);
        }
    }

    void closeIssue(const std::string& issueId) {
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
