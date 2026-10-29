#ifndef AGENT_REPOSITORY_HPP
#define AGENT_REPOSITORY_HPP

#include <unordered_map>
#include <vector>
#include <mutex>
#include "../model/Agent.hpp"

class AgentRepository {
private:
    std::unordered_map<std::string, Agent*> agents;
    std::vector<Agent*> agentList;  // Maintain insertion order
    mutable std::mutex mutex_;

public:
    void addAgent(Agent* agent) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (agents.find(agent->getAgentId()) == agents.end()) {
            agents[agent->getAgentId()] = agent;
            agentList.push_back(agent);
        }
    }

    void removeAgent(const std::string& agentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = agents.find(agentId);
        if (it != agents.end()) {
            Agent* agent = it->second;
            agents.erase(it);
            agentList.erase(
                std::remove(agentList.begin(), agentList.end(), agent),
                agentList.end()
            );
        }
    }

    Agent* getAgent(const std::string& agentId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = agents.find(agentId);
        return (it != agents.end()) ? it->second : nullptr;
    }

    std::vector<Agent*>& getAllAgents() {
        return agentList;
    }

    std::vector<Agent*> getAvailableAgents() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Agent*> available;
        for (Agent* agent : agentList) {
            if (agent->isAvailable()) {
                available.push_back(agent);
            }
        }
        return available;
    }

    std::vector<Agent*> getAgentsByExpertise(IssueType type) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<Agent*> experts;
        for (Agent* agent : agentList) {
            if (agent->hasExpertise(type)) {
                experts.push_back(agent);
            }
        }
        return experts;
    }

    int getAgentCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<int>(agents.size());
    }
};

#endif // AGENT_REPOSITORY_HPP
