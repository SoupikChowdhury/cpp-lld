#pragma once

#include <memory>
#include <vector>
#include <string>
#include "User/User.hpp"
#include "User/UserController.hpp"
#include "Group/Group.hpp"
#include "Group/GroupController.hpp"
#include "BalanceSheetController.hpp"
#include "Expense/ExpenseSplitType.hpp"
#include "Expense/Split/Split.hpp"

/**
 * @brief Main Splitwise application class
 * 
 * This is the facade that provides a simplified interface to the
 * expense sharing system. It manages users, groups, and balance sheets.
 */
class Splitwise {
private:
    UserController userController;
    GroupController groupController;
    BalanceSheetController balanceSheetController;

    /**
     * @brief Add initial users to the Splitwise app
     */
    void addUsersToSplitwiseApp() {
        // Adding User1
        auto user1 = std::make_shared<User>("U1001", "User1");
        
        // Adding User2
        auto user2 = std::make_shared<User>("U2001", "User2");
        
        // Adding User3
        auto user3 = std::make_shared<User>("U3001", "User3");

        userController.addUser(user1);
        userController.addUser(user2);
        userController.addUser(user3);
    }

public:
    Splitwise() = default;

    /**
     * @brief Set up users and initial group
     */
    void setupUserAndGroup() {
        // Onboard users to splitwise app
        addUsersToSplitwiseApp();

        // Create a group by user1
        auto user1 = userController.getUser("U1001");
        groupController.createNewGroup("G1001", "Outing with Friends", user1);
    }

    /**
     * @brief Run a demo of the Splitwise functionality
     */
    void demo() {
        setupUserAndGroup();

        // Step 1: Add members to the group
        auto group = groupController.getGroup("G1001");
        group->addMember(userController.getUser("U2001"));
        group->addMember(userController.getUser("U3001"));

        // Step 2: Create an expense inside a group (EQUAL split)
        std::vector<std::shared_ptr<Split>> splits;
        splits.push_back(std::make_shared<Split>(userController.getUser("U1001"), 300));
        splits.push_back(std::make_shared<Split>(userController.getUser("U2001"), 300));
        splits.push_back(std::make_shared<Split>(userController.getUser("U3001"), 300));
        
        group->createExpense(
            "Exp1001", "Breakfast", 900, 
            splits, ExpenseSplitType::EQUAL, 
            userController.getUser("U1001"));

        // Step 3: Create another expense (UNEQUAL split)
        std::vector<std::shared_ptr<Split>> splits2;
        splits2.push_back(std::make_shared<Split>(userController.getUser("U1001"), 400));
        splits2.push_back(std::make_shared<Split>(userController.getUser("U2001"), 100));
        
        group->createExpense(
            "Exp1002", "Lunch", 500, 
            splits2, ExpenseSplitType::UNEQUAL, 
            userController.getUser("U2001"));

        // Display balance sheets for all users
        for (const auto& user : userController.getAllUsers()) {
            balanceSheetController.showBalanceSheetOfUser(user);
        }
    }

    // Accessor methods for extending functionality
    UserController& getUserController() { return userController; }
    GroupController& getGroupController() { return groupController; }
    BalanceSheetController& getBalanceSheetController() { return balanceSheetController; }
};
