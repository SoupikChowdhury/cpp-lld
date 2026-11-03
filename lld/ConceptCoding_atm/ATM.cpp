#include "ATM.hpp"
#include "States/IdleState.hpp"
#include "States/HasCardState.hpp"
#include "States/SelectOperationState.hpp"
#include "States/CashWithdrawalState.hpp"
#include "States/CheckBalanceState.hpp"
#include "Withdrawal/TwoThousandWithdrawProcessor.hpp"
#include "Withdrawal/FiveHundredWithdrawProcessor.hpp"
#include "Withdrawal/OneHundredWithdrawProcessor.hpp"
#include <algorithm>

// Static instance
ATM* ATM::instance = nullptr;

ATM* ATM::getATMObject() {
    if (instance == nullptr) {
        instance = new ATM();
        instance->setCurrentATMState(std::make_shared<IdleState>());
    }
    return instance;
}

// IdleState implementation
void IdleState::insertCard(ATM* atm, std::shared_ptr<Card> card) {
    std::cout << "Card is inserted" << std::endl;
    atm->setCurrentATMState(std::make_shared<HasCardState>());
}

// HasCardState implementation
void HasCardState::authenticatePin(ATM* atm, std::shared_ptr<Card> card, int pin) {
    if (card->isCorrectPINEntered(pin)) {
        atm->setCurrentATMState(std::make_shared<SelectOperationState>());
    } else {
        std::cout << "Invalid PIN Number" << std::endl;
        exit(atm);
    }
}

void HasCardState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(std::make_shared<IdleState>());
    std::cout << "Exit happens" << std::endl;
}

// SelectOperationState implementation
void SelectOperationState::selectOperation(ATM* atm, std::shared_ptr<Card> card, TransactionType txnType) {
    switch (txnType) {
        case TransactionType::CASH_WITHDRAWAL:
            atm->setCurrentATMState(std::make_shared<CashWithdrawalState>());
            break;
        case TransactionType::BALANCE_CHECK:
            atm->setCurrentATMState(std::make_shared<CheckBalanceState>());
            break;
        default:
            std::cout << "Invalid Option" << std::endl;
            exit(atm);
    }
}

void SelectOperationState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(std::make_shared<IdleState>());
    std::cout << "Exit happens" << std::endl;
}

// Dry run of the 2000 -> 500 -> 100 chain: can the ATM's notes make exactly this amount?
static bool canDispenseExactly(const ATM* atm, int amount) {
    const int values[] = {2000, 500, 100};
    const int notes[] = {atm->getNoOfTwoThousandNotes(), atm->getNoOfFiveHundredNotes(),
                         atm->getNoOfOneHundredNotes()};
    for (int i = 0; i < 3; ++i) {
        int use = std::min(amount / values[i], notes[i]);
        amount -= use * values[i];
    }
    return amount == 0;
}

// CashWithdrawalState implementation
void CashWithdrawalState::cashWithdrawal(ATM* atm, std::shared_ptr<Card> card, int withdrawAmount) {
    if (withdrawAmount <= 0) {
        std::cout << "Invalid amount: " << withdrawAmount << std::endl;
        exit(atm);
    } else if (atm->getAtmBalance() < withdrawAmount) {
        std::cout << "Insufficient fund in the ATM Machine" << std::endl;
        exit(atm);
    } else if (card->getBankBalance() < withdrawAmount) {
        std::cout << "Insufficient fund in your Bank Account" << std::endl;
        exit(atm);
    } else if (!canDispenseExactly(atm, withdrawAmount)) {
        // Check before debiting anything, so nobody is charged for cash that can't come out
        std::cout << "Cannot dispense this exact amount with the available notes" << std::endl;
        exit(atm);
    } else {
        card->deductBankBalance(withdrawAmount);
        atm->deductATMBalance(withdrawAmount);

        // Chain of Responsibility for denomination selection
        auto withdrawProcessor = std::make_shared<TwoThousandWithdrawProcessor>(
            std::make_shared<FiveHundredWithdrawProcessor>(
                std::make_shared<OneHundredWithdrawProcessor>(nullptr)
            )
        );

        withdrawProcessor->withdraw(atm, withdrawAmount);
        exit(atm);
    }
}

void CashWithdrawalState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(std::make_shared<IdleState>());
    std::cout << "Exit happens" << std::endl;
}

// CheckBalanceState implementation
void CheckBalanceState::displayBalance(ATM* atm, std::shared_ptr<Card> card) {
    std::cout << "Your Balance is: " << card->getBankBalance() << std::endl;
    exit(atm);
}

void CheckBalanceState::exit(ATM* atm) {
    returnCard();
    atm->setCurrentATMState(std::make_shared<IdleState>());
    std::cout << "Exit happens" << std::endl;
}

// Cash Withdrawal Processors implementations
void CashWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    if (nextProcessor != nullptr) {
        nextProcessor->withdraw(atm, remainingAmount);
    }
}

void TwoThousandWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    int required = remainingAmount / 2000;
    int balance = remainingAmount % 2000;

    if (required <= atm->getNoOfTwoThousandNotes()) {
        atm->deductTwoThousandNotes(required);
        std::cout << "Dispensing " << required << " notes of 2000" << std::endl;
    } else if (required > atm->getNoOfTwoThousandNotes()) {
        int available = atm->getNoOfTwoThousandNotes();
        atm->deductTwoThousandNotes(available);
        std::cout << "Dispensing " << available << " notes of 2000" << std::endl;
        balance = balance + (required - available) * 2000;
    }

    if (balance != 0) {
        CashWithdrawProcessor::withdraw(atm, balance);
    }
}

void FiveHundredWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    int required = remainingAmount / 500;
    int balance = remainingAmount % 500;

    if (required <= atm->getNoOfFiveHundredNotes()) {
        atm->deductFiveHundredNotes(required);
        std::cout << "Dispensing " << required << " notes of 500" << std::endl;
    } else if (required > atm->getNoOfFiveHundredNotes()) {
        int available = atm->getNoOfFiveHundredNotes();
        atm->deductFiveHundredNotes(available);
        std::cout << "Dispensing " << available << " notes of 500" << std::endl;
        balance = balance + (required - available) * 500;
    }

    if (balance != 0) {
        CashWithdrawProcessor::withdraw(atm, balance);
    }
}

void OneHundredWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    int required = remainingAmount / 100;
    int balance = remainingAmount % 100;

    if (required <= atm->getNoOfOneHundredNotes()) {
        atm->deductOneHundredNotes(required);
        std::cout << "Dispensing " << required << " notes of 100" << std::endl;
    } else if (required > atm->getNoOfOneHundredNotes()) {
        int available = atm->getNoOfOneHundredNotes();
        atm->deductOneHundredNotes(available);
        std::cout << "Dispensing " << available << " notes of 100" << std::endl;
        balance = balance + (required - available) * 100;
    }

    if (balance != 0) {
        std::cout << "Cannot dispense remaining: " << balance << std::endl;
    }
}
