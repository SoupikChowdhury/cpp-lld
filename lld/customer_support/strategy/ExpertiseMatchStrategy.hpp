#ifndef EXPERTISE_MATCH_STRATEGY_HPP
#define EXPERTISE_MATCH_STRATEGY_HPP

#include "AssignmentStrategy.hpp"

class ExpertiseMatchStrategy : public AssignmentStrategy {
public:
    Agent* assignIssue(std::vector<Agent*>& agents, Issue* issue) override {
        if (agents.empty()) return nullptr;

        IssueType issueType = issue->getType();

        // First pass: find available agent with matching expertise
        for (Agent* agent : agents) {
            if (agent->isAvailable() && agent->hasExpertise(issueType)) {
                return agent;
            }
        }

        // Second pass: find any available agent
        for (Agent* agent : agents) {
            if (agent->isAvailable()) {
                return agent;
            }
        }

        return nullptr;  // No available agent found
    }

    std::string getStrategyName() const override {
        return "ExpertiseMatchStrategy";
    }
};

#endif // EXPERTISE_MATCH_STRATEGY_HPP
