#ifndef AGENT_STATUS_HPP
#define AGENT_STATUS_HPP

#include <string>

enum class AgentStatus {
    AVAILABLE,
    BUSY,
    OFFLINE
};

inline std::string agentStatusToString(AgentStatus status) {
    switch (status) {
        case AgentStatus::AVAILABLE: return "AVAILABLE";
        case AgentStatus::BUSY: return "BUSY";
        case AgentStatus::OFFLINE: return "OFFLINE";
        default: return "UNKNOWN";
    }
}

#endif // AGENT_STATUS_HPP
