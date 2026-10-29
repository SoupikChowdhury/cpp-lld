#include "ATM.hpp"
#include "User.hpp"
#include <iostream>

int main() {
    std::cout << "=== ATM System Demo ===" << std::endl;

    // Initialize ATM
    ATM* atm = ATM::getATMObject();
    atm->setAtmBalance(3500, 1, 2, 5);  // 1x2000 + 2x500 + 5x100 = 3500

    // Create user with bank account and card
    auto bankAccount = std::make_shared<UserBankAccount>();
    bankAccount->balance = 5000;

    auto card = std::make_shared<Card>();
    card->setBankAccount(bankAccount);

    User user;
    user.setCard(card);

    std::cout << "\n--- Initial ATM Status ---" << std::endl;
    atm->printCurrentATMStatus();

    std::cout << "\n--- User inserts card ---" << std::endl;
    atm->getCurrentATMState()->insertCard(atm, user.getCard());

    std::cout << "\n--- User enters PIN ---" << std::endl;
    atm->getCurrentATMState()->authenticatePin(atm, user.getCard(), 112211);

    std::cout << "\n--- User selects Cash Withdrawal ---" << std::endl;
    atm->getCurrentATMState()->selectOperation(atm, user.getCard(), TransactionType::CASH_WITHDRAWAL);

    std::cout << "\n--- User withdraws 2700 ---" << std::endl;
    atm->getCurrentATMState()->cashWithdrawal(atm, user.getCard(), 2700);

    std::cout << "\n--- Final ATM Status ---" << std::endl;
    atm->printCurrentATMStatus();

    std::cout << "\n--- User Bank Balance: " << bankAccount->balance << " ---" << std::endl;

    return 0;
}
