// Splitwise - Main Entry Point
// Demonstrates Strategy, Factory, and Repository patterns

#include <iostream>
#include <vector>
#include "model/User.hpp"
#include "repository/GroupRepository.hpp"
#include "service/GroupService.hpp"

int main() {
    std::cout << "=== Splitwise Demo ===" << std::endl;
    std::cout << std::endl;

    // Create users
    User alice("u1", "Alice");
    User bob("u2", "Bob");
    User charlie("u3", "Charlie");
    User dave("u4", "Dave");

    // Create repository and service
    GroupRepository repository;
    GroupService service(repository);

    // Create a group
    std::vector<User*> members = {&alice, &bob, &charlie, &dave};
    std::string groupId = service.createGroup("Trip to Goa", members);

    std::cout << std::endl << "--- Adding Expenses ---" << std::endl;

    // Add expenses
    // Alice pays for hotel - split equally
    service.addExpense(groupId, "Hotel Stay", 4000, &alice, 
                       {&alice, &bob, &charlie, &dave}, 
                       SplitType::EQUAL);

    // Bob pays for dinner - split equally
    service.addExpense(groupId, "Dinner", 2000, &bob, 
                       {&alice, &bob, &charlie, &dave}, 
                       SplitType::EQUAL);

    // Charlie pays for cab - percentage split
    std::unordered_map<std::string, double> cabSplit = {
        {"u1", 25}, {"u2", 25}, {"u3", 25}, {"u4", 25}
    };
    service.addExpense(groupId, "Cab Ride", 800, &charlie, 
                       {&alice, &bob, &charlie, &dave}, 
                       SplitType::PERCENTAGE, cabSplit);

    // Dave pays for activities - split equally
    service.addExpense(groupId, "Water Sports", 1200, &dave, 
                       {&alice, &bob, &charlie, &dave}, 
                       SplitType::EQUAL);

    // Print balances
    service.printBalances(groupId);

    // Simplify debts
    service.simplifyDebts(groupId);

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
