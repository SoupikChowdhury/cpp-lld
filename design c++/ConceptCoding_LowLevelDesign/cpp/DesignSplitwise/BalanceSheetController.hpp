#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "Balance.hpp"
#include "UserExpenseBalanceSheet.hpp"
#include "User/User.hpp"
#include "Expense/Split/Split.hpp"

/**
 * @brief Controller for managing user balance sheets
 * 
 * Handles updating balance sheets when expenses are created
 * and displaying balance information for users.
 */
class BalanceSheetController {
public:
    /**
     * @brief Update balance sheets for all users involved in an expense
     * @param expensePaidBy User who paid for the expense
     * @param splits List of splits defining how expense is divided
     * @param totalExpenseAmount Total amount of the expense
     */
    void updateUserExpenseBalanceSheet(
        std::shared_ptr<User> expensePaidBy,
        const std::vector<std::shared_ptr<Split>>& splits,
        double totalExpenseAmount) {
        
        // Update the total payment made by the payer
        auto paidByUserExpenseSheet = expensePaidBy->getUserExpenseBalanceSheet();
        paidByUserExpenseSheet->setTotalPayment(
            paidByUserExpenseSheet->getTotalPayment() + totalExpenseAmount);

        for (const auto& split : splits) {
            auto userOwe = split->getUser();
            auto oweUserExpenseSheet = userOwe->getUserExpenseBalanceSheet();
            double oweAmount = split->getAmountOwe();

            if (expensePaidBy->getUserId() == userOwe->getUserId()) {
                // The payer's own expense portion
                paidByUserExpenseSheet->setTotalYourExpense(
                    paidByUserExpenseSheet->getTotalYourExpense() + oweAmount);
            } else {
                // Update payer's balance (they should get back this amount)
                paidByUserExpenseSheet->setTotalYouGetBack(
                    paidByUserExpenseSheet->getTotalYouGetBack() + oweAmount);

                // Update or create balance entry for the user who owes
                auto& paidByUserBalanceMap = paidByUserExpenseSheet->getUserVsBalance();
                std::shared_ptr<Balance> userOweBalance;
                
                auto it = paidByUserBalanceMap.find(userOwe->getUserId());
                if (it != paidByUserBalanceMap.end()) {
                    userOweBalance = it->second;
                } else {
                    userOweBalance = std::make_shared<Balance>();
                    paidByUserBalanceMap[userOwe->getUserId()] = userOweBalance;
                }
                userOweBalance->setAmountGetBack(
                    userOweBalance->getAmountGetBack() + oweAmount);

                // Update the owing user's balance sheet
                oweUserExpenseSheet->setTotalYouOwe(
                    oweUserExpenseSheet->getTotalYouOwe() + oweAmount);
                oweUserExpenseSheet->setTotalYourExpense(
                    oweUserExpenseSheet->getTotalYourExpense() + oweAmount);

                // Update or create balance entry for the payer in owe user's sheet
                auto& oweUserBalanceMap = oweUserExpenseSheet->getUserVsBalance();
                std::shared_ptr<Balance> userPaidBalance;
                
                auto it2 = oweUserBalanceMap.find(expensePaidBy->getUserId());
                if (it2 != oweUserBalanceMap.end()) {
                    userPaidBalance = it2->second;
                } else {
                    userPaidBalance = std::make_shared<Balance>();
                    oweUserBalanceMap[expensePaidBy->getUserId()] = userPaidBalance;
                }
                userPaidBalance->setAmountOwe(
                    userPaidBalance->getAmountOwe() + oweAmount);
            }
        }
    }

    /**
     * @brief Display the balance sheet of a user
     * @param user The user whose balance sheet to display
     */
    void showBalanceSheetOfUser(std::shared_ptr<User> user) {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Balance sheet of user: " << user->getUserId() 
                  << " (" << user->getUserName() << ")" << std::endl;

        auto userExpenseBalanceSheet = user->getUserExpenseBalanceSheet();

        std::cout << "TotalYourExpense: " << userExpenseBalanceSheet->getTotalYourExpense() << std::endl;
        std::cout << "TotalGetBack: " << userExpenseBalanceSheet->getTotalYouGetBack() << std::endl;
        std::cout << "TotalYouOwe: " << userExpenseBalanceSheet->getTotalYouOwe() << std::endl;
        std::cout << "TotalPaymentMade: " << userExpenseBalanceSheet->getTotalPayment() << std::endl;

        const auto& balanceMap = userExpenseBalanceSheet->getUserVsBalance();
        for (const auto& entry : balanceMap) {
            const std::string& userID = entry.first;
            const auto& balance = entry.second;
            std::cout << "  UserID: " << userID 
                      << " YouGetBack: " << balance->getAmountGetBack() 
                      << " YouOwe: " << balance->getAmountOwe() << std::endl;
        }
    }
};
