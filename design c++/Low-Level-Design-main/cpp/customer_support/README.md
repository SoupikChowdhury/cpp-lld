# Customer Support System - Low Level Design (C++)

## Design Patterns Used
- **Strategy Pattern**: For agent assignment strategies
- **Factory Pattern**: For creating agents and issues
- **Observer Pattern**: For notifying agents about new issues (optional)
- **Repository Pattern**: For managing agents and issues

## Class Diagram

```mermaid
classDiagram
    class IssueType {
        <<enumeration>>
        BILLING
        TECHNICAL
        GENERAL
        ACCOUNT
    }

    class IssueStatus {
        <<enumeration>>
        OPEN
        IN_PROGRESS
        RESOLVED
        CLOSED
    }

    class AgentStatus {
        <<enumeration>>
        AVAILABLE
        BUSY
        OFFLINE
    }

    class Issue {
        -string issueId
        -string customerId
        -string description
        -IssueType type
        -IssueStatus status
        -time_t createdAt
        -time_t resolvedAt
        +getIssueId() string
        +getType() IssueType
        +getStatus() IssueStatus
        +setStatus(IssueStatus)
        +resolve()
        +close()
    }

    class Agent {
        -string agentId
        -string name
        -AgentStatus status
        -vector~IssueType~ expertise
        -Issue* currentIssue
        +getAgentId() string
        +getName() string
        +isAvailable() bool
        +hasExpertise(IssueType) bool
        +assignIssue(Issue*)
        +resolveCurrentIssue()
        +getStatus() AgentStatus
    }

    class AssignmentStrategy {
        <<interface>>
        +assignIssue(agents, issue) Agent*
    }

    class RoundRobinStrategy {
        -int lastAssignedIndex
        +assignIssue(agents, issue) Agent*
    }

    class ExpertiseMatchStrategy {
        +assignIssue(agents, issue) Agent*
    }

    class LeastLoadedStrategy {
        +assignIssue(agents, issue) Agent*
    }

    AssignmentStrategy <|.. RoundRobinStrategy
    AssignmentStrategy <|.. ExpertiseMatchStrategy
    AssignmentStrategy <|.. LeastLoadedStrategy

    class AgentRepository {
        -map~string,Agent*~ agents
        +addAgent(Agent*)
        +removeAgent(agentId)
        +getAgent(agentId) Agent*
        +getAvailableAgents() vector~Agent*~
        +getAgentsByExpertise(IssueType) vector~Agent*~
    }

    class IssueRepository {
        -map~string,Issue*~ issues
        -queue~Issue*~ waitlist
        +createIssue(customerId, description, type) Issue*
        +getIssue(issueId) Issue*
        +addToWaitlist(Issue*)
        +getNextFromWaitlist() Issue*
        +hasWaitingIssues() bool
    }

    class CustomerSupportService {
        -AgentRepository agentRepo
        -IssueRepository issueRepo
        -AssignmentStrategy* strategy
        +raiseIssue(customerId, description, type) Issue*
        +assignIssueToAgent(issueId)
        +resolveIssue(agentId)
        +setAssignmentStrategy(strategy)
        +getWaitlistSize() int
    }

    CustomerSupportService --> AgentRepository
    CustomerSupportService --> IssueRepository
    CustomerSupportService --> AssignmentStrategy
    AgentRepository --> Agent
    IssueRepository --> Issue
    Agent --> Issue : handles
```

## Issue Assignment Flow

```mermaid
sequenceDiagram
    participant C as Customer
    participant CSS as CustomerSupportService
    participant IR as IssueRepository
    participant AR as AgentRepository
    participant AS as AssignmentStrategy
    participant A as Agent

    C->>CSS: raiseIssue(customerId, description, type)
    CSS->>IR: createIssue(...)
    IR-->>CSS: Issue
    CSS->>AR: getAvailableAgents()
    AR-->>CSS: List<Agent>
    
    alt Agents Available
        CSS->>AS: assignIssue(agents, issue)
        AS-->>CSS: selectedAgent
        CSS->>A: assignIssue(issue)
        A->>A: setStatus(BUSY)
        CSS-->>C: Issue assigned to agent
    else No Agents Available
        CSS->>IR: addToWaitlist(issue)
        CSS-->>C: Issue in queue
    end
```

## Issue Resolution and Waitlist Processing

```mermaid
flowchart TD
    A[Agent Resolves Issue] --> B[Mark Issue RESOLVED]
    B --> C[Set Agent AVAILABLE]
    C --> D{Waitlist Empty?}
    D -->|No| E[Get Next Issue from Waitlist]
    E --> F[Assign to Agent]
    F --> G[Agent becomes BUSY]
    D -->|Yes| H[Agent Remains Available]
```

## State Diagram - Issue Lifecycle

```mermaid
stateDiagram-v2
    [*] --> OPEN : Issue Created
    OPEN --> IN_PROGRESS : Assigned to Agent
    OPEN --> OPEN : In Waitlist
    IN_PROGRESS --> RESOLVED : Agent Resolves
    RESOLVED --> CLOSED : Customer Confirms
    CLOSED --> [*]
```

## State Diagram - Agent Status

```mermaid
stateDiagram-v2
    [*] --> AVAILABLE : Agent Joins
    AVAILABLE --> BUSY : Issue Assigned
    BUSY --> AVAILABLE : Issue Resolved
    AVAILABLE --> OFFLINE : Agent Logs Out
    BUSY --> OFFLINE : Agent Logs Out
    OFFLINE --> AVAILABLE : Agent Logs In
    OFFLINE --> [*] : Agent Removed
```

## Assignment Strategies

```mermaid
flowchart TD
    A[Select Agent] --> B{Which Strategy?}
    
    B -->|Round Robin| C[RoundRobinStrategy]
    C --> D[Get next agent in circular order]
    D --> E[Skip if not available]
    
    B -->|Expertise Match| F[ExpertiseMatchStrategy]
    F --> G[Filter agents by issue type expertise]
    G --> H[Return first available expert]
    
    B -->|Least Loaded| I[LeastLoadedStrategy]
    I --> J[Count resolved issues per agent]
    J --> K[Return agent with least issues]
```

## Files Structure
```
customer_support/
├── enums/
│   ├── IssueType.hpp
│   ├── IssueStatus.hpp
│   └── AgentStatus.hpp
├── model/
│   ├── Issue.hpp
│   └── Agent.hpp
├── strategy/
│   ├── AssignmentStrategy.hpp
│   ├── RoundRobinStrategy.hpp
│   ├── ExpertiseMatchStrategy.hpp
│   └── LeastLoadedStrategy.hpp
├── repository/
│   ├── AgentRepository.hpp
│   └── IssueRepository.hpp
├── service/
│   └── CustomerSupportService.hpp
├── README.md
└── main.cpp
```
