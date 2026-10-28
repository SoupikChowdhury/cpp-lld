#ifndef LEAST_LOADED_STRATEGY_HPP
#define LEAST_LOADED_STRATEGY_HPP

#include "AssignmentStrategy.hpp"
#include <limits>

class LeastLoadedStrategy : public AssignmentStrategy {
public:
    Agent* assignIssue(std::vector<Agent*>& agents, Issue* issue) override {
        if (agents.empty()) return nullptr;

        Agent* leastLoaded = nullptr;
        int minResolved = std::numeric_limits<int>::max();

        for (Agent* agent : agents) {
            if (agent->isAvailable()) {
                if (agent->getResolvedCount() < minResolved) {
                    minResolved = agent->getResolvedCount();
                    leastLoaded = agent;
                }
            }
        }

        return leastLoaded;
    }

    std::string getStrategyName() const override {
        return "LeastLoadedStrategy";
    }
};

#endif // LEAST_LOADED_STRATEGY_HPP
