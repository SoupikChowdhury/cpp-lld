#ifndef ASSIGNMENT_STRATEGY_HPP
#define ASSIGNMENT_STRATEGY_HPP

#include <vector>
#include "../model/Agent.hpp"
#include "../model/Issue.hpp"

class AssignmentStrategy {
public:
    virtual ~AssignmentStrategy() = default;
    virtual Agent* assignIssue(std::vector<Agent*>& agents, Issue* issue) = 0;
    virtual std::string getStrategyName() const = 0;
};

#endif // ASSIGNMENT_STRATEGY_HPP
