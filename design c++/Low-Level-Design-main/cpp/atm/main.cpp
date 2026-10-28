// ATM Machine - Main Entry Point
// Demonstrates State Pattern and Chain of Responsibility

#include <iostream>
#include <memory>
#include "model/Account.hpp"
#include "model/Card.hpp"
#include "model/ATM.hpp"
#include "repository/ATMRepository.hpp"
#include "service/ATMMachine.hpp"


int main() {
    std::cout << "=== ATM Machine Demo ===" << std::endl;
    std::cout << std::endl;

    // Create account and card
    auto account = std::make_shared<Account>("ACC123", 5000);
    auto card = std::make_shared<Card>("CARD123", "1234", account);

    // Create ATMs
    auto atm1 = std::make_shared<ATM>("ATM1", 5, 5, 20);  // 5x2000 + 5x500 + 20x100 = 14500
    auto atm2 = std::make_shared<ATM>("ATM2", 0, 2, 5);   // 0x2000 + 2x500 + 5x100 = 1500

    // Create repository and save ATMs
    auto atmRepository = std::make_shared<ATMRepository>();
    atmRepository->save(atm1);
    atmRepository->save(atm2);

    std::cout << "ATM2 Cash Available: " << atm2->getCashAvailable() << std::endl;
    std::cout << "Account Balance: " << account->getBalance() << std::endl;
    std::cout << std::endl;

    // Create ATM Machine
    ATMMachine atmMachine("ATM2", atmRepository);

    // Perform operations
    std::cout << "--- Inserting Card ---" << std::endl;
    atmMachine.insertCard(card);

    std::cout << std::endl << "--- Entering PIN ---" << std::endl;
    atmMachine.enterPin("1234");

    std::cout << std::endl << "--- Selecting Withdrawal ---" << std::endl;
    atmMachine.selectOption("WITHDRAW");

    std::cout << std::endl << "--- Dispensing Cash (1100) ---" << std::endl;
    atmMachine.dispenseCash(1100);

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
