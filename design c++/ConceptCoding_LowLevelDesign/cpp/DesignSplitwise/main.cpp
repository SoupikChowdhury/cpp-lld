#include <iostream>
#include <memory>
#include <vector>
#include "Splitwise.hpp"
#include "Expense/ExpenseSplitType.hpp"
#include "Expense/Split/Split.hpp"

/**
 * @brief Demonstrates the Splitwise expense sharing application
 * 
 * This demo shows:
 * 1. Creating users and adding them to the system
 * 2. Creating a group with members
 * 3. Adding expenses with different split types (EQUAL, UNEQUAL)
 * 4. Viewing balance sheets for each user
 */
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "    SPLITWISE - Expense Sharing App    " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Create the Splitwise application
    Splitwise splitwise;
    
    // Run the demo
    std::cout << "Running Splitwise Demo..." << std::endl;
    std::cout << std::endl;
    
    splitwise.demo();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "         Extended Demo Example         " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Extended demo with a new Splitwise instance
    Splitwise splitwise2;
    
    // Create users
    auto& userController = splitwise2.getUserController();
    userController.addUser(std::make_shared<User>("U101", "Alice"));
    userController.addUser(std::make_shared<User>("U102", "Bob"));
    userController.addUser(std::make_shared<User>("U103", "Charlie"));
    userController.addUser(std::make_shared<User>("U104", "Diana"));

    // Create a group
    auto& groupController = splitwise2.getGroupController();
    groupController.createNewGroup("G101", "Weekend Trip", userController.getUser("U101"));
    
    auto group = groupController.getGroup("G101");
    group->addMember(userController.getUser("U102"));
    group->addMember(userController.getUser("U103"));
    group->addMember(userController.getUser("U104"));

    std::cout << "Group Created: Weekend Trip" << std::endl;
    std::cout << "Members: Alice, Bob, Charlie, Diana" << std::endl;
    std::cout << std::endl;

    // Expense 1: Hotel (Equal split - $400 total, $100 each)
    std::cout << "Expense 1: Hotel - $400 (Equal split)" << std::endl;
    std::cout << "  Paid by: Alice" << std::endl;
    std::vector<std::shared_ptr<Split>> hotelSplits;
    hotelSplits.push_back(std::make_shared<Split>(userController.getUser("U101"), 100));
    hotelSplits.push_back(std::make_shared<Split>(userController.getUser("U102"), 100));
    hotelSplits.push_back(std::make_shared<Split>(userController.getUser("U103"), 100));
    hotelSplits.push_back(std::make_shared<Split>(userController.getUser("U104"), 100));
    
    group->createExpense(
        "E101", "Hotel Accommodation", 400,
        hotelSplits, ExpenseSplitType::EQUAL,
        userController.getUser("U101"));

    // Expense 2: Dinner (Unequal split - $150 total)
    std::cout << "Expense 2: Dinner - $150 (Unequal split)" << std::endl;
    std::cout << "  Paid by: Bob" << std::endl;
    std::cout << "  Alice: $30, Bob: $50, Charlie: $40, Diana: $30" << std::endl;
    std::vector<std::shared_ptr<Split>> dinnerSplits;
    dinnerSplits.push_back(std::make_shared<Split>(userController.getUser("U101"), 30));
    dinnerSplits.push_back(std::make_shared<Split>(userController.getUser("U102"), 50));
    dinnerSplits.push_back(std::make_shared<Split>(userController.getUser("U103"), 40));
    dinnerSplits.push_back(std::make_shared<Split>(userController.getUser("U104"), 30));
    
    group->createExpense(
        "E102", "Dinner at Restaurant", 150,
        dinnerSplits, ExpenseSplitType::UNEQUAL,
        userController.getUser("U102"));

    // Expense 3: Taxi (Equal split - $80 total)
    std::cout << "Expense 3: Taxi - $80 (Equal split)" << std::endl;
    std::cout << "  Paid by: Charlie" << std::endl;
    std::vector<std::shared_ptr<Split>> taxiSplits;
    taxiSplits.push_back(std::make_shared<Split>(userController.getUser("U101"), 20));
    taxiSplits.push_back(std::make_shared<Split>(userController.getUser("U102"), 20));
    taxiSplits.push_back(std::make_shared<Split>(userController.getUser("U103"), 20));
    taxiSplits.push_back(std::make_shared<Split>(userController.getUser("U104"), 20));
    
    group->createExpense(
        "E103", "Taxi to Airport", 80,
        taxiSplits, ExpenseSplitType::EQUAL,
        userController.getUser("U103"));

    std::cout << std::endl;
    std::cout << "Balance Sheets After All Expenses:" << std::endl;

    // Display balance sheets
    auto& balanceController = splitwise2.getBalanceSheetController();
    for (const auto& user : userController.getAllUsers()) {
        balanceController.showBalanceSheetOfUser(user);
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "          Demo Complete!               " << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
