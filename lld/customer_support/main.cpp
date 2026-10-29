// Customer Support System - Main Entry Point
// Demonstrates Strategy, Repository patterns

#include <iostream>
#include "model/Agent.hpp"
#include "model/Issue.hpp"
#include "service/CustomerSupportService.hpp"
#include "strategy/RoundRobinStrategy.hpp"
#include "strategy/ExpertiseMatchStrategy.hpp"
#include "strategy/LeastLoadedStrategy.hpp"

int main() {
    std::cout << "=== Customer Support System Demo ===" << std::endl;
    std::cout << std::endl;

    CustomerSupportService supportService;

    // Create agents with expertise
    Agent* alice = new Agent("A001", "Alice", {IssueType::BILLING, IssueType::ACCOUNT});
    Agent* bob = new Agent("A002", "Bob", {IssueType::TECHNICAL});
    Agent* charlie = new Agent("A003", "Charlie", {});  // Can handle all types

    supportService.addAgent(alice);
    supportService.addAgent(bob);
    supportService.addAgent(charlie);

    std::cout << "--- Agents Registered ---" << std::endl;
    for (Agent* agent : supportService.getAllAgents()) {
        std::cout << "  " << agent->getAgentId() << " - " << agent->getName()
                  << " | Expertise: " << agent->getExpertiseString()
                  << " | Status: " << agentStatusToString(agent->getStatus()) << std::endl;
    }

    supportService.printStatus();

    // Test with Round Robin Strategy (default)
    std::cout << std::endl << "--- Raising Issues (Round Robin Strategy) ---" << std::endl;
    
    Issue* issue1 = supportService.raiseIssue("C001", "Cannot login to my account", IssueType::ACCOUNT);
    Issue* issue2 = supportService.raiseIssue("C002", "Payment not processed", IssueType::BILLING);
    Issue* issue3 = supportService.raiseIssue("C003", "App crashes on startup", IssueType::TECHNICAL);

    std::cout << std::endl << "--- Agent Status After Assignments ---" << std::endl;
    for (Agent* agent : supportService.getAllAgents()) {
        std::cout << "  " << agent->getName() << " | Status: " 
                  << agentStatusToString(agent->getStatus());
        if (agent->getCurrentIssue()) {
            std::cout << " | Handling: " << agent->getCurrentIssue()->getIssueId();
        }
        std::cout << std::endl;
    }

    // More issues - should go to waitlist
    std::cout << std::endl << "--- Raising More Issues (All agents busy) ---" << std::endl;
    Issue* issue4 = supportService.raiseIssue("C004", "Refund not received", IssueType::BILLING);
    Issue* issue5 = supportService.raiseIssue("C005", "Feature request", IssueType::GENERAL);

    supportService.printStatus();

    // Resolve some issues
    std::cout << std::endl << "--- Resolving Issues ---" << std::endl;
    supportService.resolveIssue("A001");  // Alice resolves, gets issue from waitlist
    supportService.resolveIssue("A002");  // Bob resolves, gets issue from waitlist

    std::cout << std::endl << "--- Agent Stats After Resolution ---" << std::endl;
    for (Agent* agent : supportService.getAllAgents()) {
        std::cout << "  " << agent->getName() 
                  << " | Status: " << agentStatusToString(agent->getStatus())
                  << " | Resolved: " << agent->getResolvedCount();
        if (agent->getCurrentIssue()) {
            std::cout << " | Current: " << agent->getCurrentIssue()->getIssueId();
        }
        std::cout << std::endl;
    }

    // Switch to Expertise Match Strategy
    std::cout << std::endl << "--- Switching to Expertise Match Strategy ---" << std::endl;
    supportService.setAssignmentStrategy(std::make_unique<ExpertiseMatchStrategy>());
    std::cout << "Active Strategy: " << supportService.getActiveStrategy() << std::endl;

    // Resolve remaining issues
    supportService.resolveIssue("A001");
    supportService.resolveIssue("A002");
    supportService.resolveIssue("A003");

    // Raise new issues with expertise matching
    std::cout << std::endl << "--- Raising Issues (Expertise Match Strategy) ---" << std::endl;
    Issue* issue6 = supportService.raiseIssue("C006", "Bug in mobile app", IssueType::TECHNICAL);
    Issue* issue7 = supportService.raiseIssue("C007", "Billing discrepancy", IssueType::BILLING);

    std::cout << std::endl << "--- Final Agent Status ---" << std::endl;
    for (Agent* agent : supportService.getAllAgents()) {
        std::cout << "  " << agent->getName() 
                  << " | Status: " << agentStatusToString(agent->getStatus())
                  << " | Resolved: " << agent->getResolvedCount();
        if (agent->getCurrentIssue()) {
            std::cout << " | Current: " << agent->getCurrentIssue()->getIssueId() 
                      << " (" << issueTypeToString(agent->getCurrentIssue()->getType()) << ")";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "--- All Issues Summary ---" << std::endl;
    for (Issue* issue : supportService.getAllIssues()) {
        std::cout << "  " << issue->getIssueId() 
                  << " | " << issueTypeToString(issue->getType())
                  << " | " << issueStatusToString(issue->getStatus())
                  << " | Agent: " << (issue->getAssignedAgentId().empty() ? "None" : issue->getAssignedAgentId())
                  << std::endl;
    }

    supportService.printStatus();

    // Cleanup
    delete alice;
    delete bob;
    delete charlie;

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
