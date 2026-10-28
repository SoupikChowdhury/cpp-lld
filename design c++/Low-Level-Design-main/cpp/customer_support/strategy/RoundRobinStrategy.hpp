#ifndef ROUND_ROBIN_STRATEGY_HPP
#define ROUND_ROBIN_STRATEGY_HPP

#include "AssignmentStrategy.hpp"

class RoundRobinStrategy : public AssignmentStrategy {
private:
    size_t lastAssignedIndex = 0;

public:
    Agent* assignIssue(std::vector<Agent*>& agents, Issue* issue) override {
        if (agents.empty()) return nullptr;

        size_t startIndex = lastAssignedIndex;
        size_t count = 0;

        while (count < agents.size()) {
            size_t currentIndex = (startIndex + count) % agents.size();
            Agent* agent = agents[currentIndex];

            if (agent->isAvailable()) {
                lastAssignedIndex = (currentIndex + 1) % agents.size();
                return agent;
            }
            count++;
        }

        return nullptr;  // No available agent found
    }

    std::string getStrategyName() const override {
        return "RoundRobinStrategy";
    }
};

#endif // ROUND_ROBIN_STRATEGY_HPP
