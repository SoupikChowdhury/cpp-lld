#include "ATM.hpp"
#include "States/IdleState.hpp"
#include "States/HasCardState.hpp"
#include "States/SelectOperationState.hpp"
#include "States/CashWithdrawalState.hpp"
#include "States/CheckBalanceState.hpp"
#include "Withdrawal/TwoThousandWithdrawProcessor.hpp"
#include "Withdrawal/FiveHundredWithdrawProcessor.hpp"
#include "Withdrawal/OneHundredWithdrawProcessor.hpp"

// Static instance
ATM* ATM::instance = nullptr;

ATM* ATM::getATMObject() {
    if (instance == nullptr) {
        instance = new ATM();
        instance-&gt;setCurrentATMState(std::make_shared&lt;IdleState&gt;());
    }
    return instance;
}

// IdleState implementation
void IdleState::insertCard(ATM* atm, std::shared_ptr&lt;Card&gt; card) {
    std::cout &lt;&lt; "Card is inserted" &lt;&lt; std::endl;
    atm-&gt;setCurrentATMState(std::make_shared&lt;HasCardState&gt;());
}

// HasCardState implementation
void HasCardState::authenticatePin(ATM* atm, std::shared_ptr&lt;Card&gt; card, int pin) {
    if (card-&gt;isCorrectPINEntered(pin)) {
        atm-&gt;setCurrentATMState(std::make_shared&lt;SelectOperationState&gt;());
    } else {
        std::cout &lt;&lt; "Invalid PIN Number" &lt;&lt; std::endl;
        exit(atm);
    }
}

void HasCardState::exit(ATM* atm) {
    returnCard();
    atm-&gt;setCurrentATMState(std::make_shared&lt;IdleState&gt;());
    std::cout &lt;&lt; "Exit happens" &lt;&lt; std::endl;
}

// SelectOperationState implementation
void SelectOperationState::selectOperation(ATM* atm, std::shared_ptr&lt;Card&gt; card, TransactionType txnType) {
    switch (txnType) {
        case TransactionType::CASH_WITHDRAWAL:
            atm-&gt;setCurrentATMState(std::make_shared&lt;CashWithdrawalState&gt;());
            break;
        case TransactionType::BALANCE_CHECK:
            atm-&gt;setCurrentATMState(std::make_shared&lt;CheckBalanceState&gt;());
            break;
        default:
            std::cout &lt;&lt; "Invalid Option" &lt;&lt; std::endl;
            exit(atm);
    }
}

void SelectOperationState::exit(ATM* atm) {
    returnCard();
    atm-&gt;setCurrentATMState(std::make_shared&lt;IdleState&gt;());
    std::cout &lt;&lt; "Exit happens" &lt;&lt; std::endl;
}

// CashWithdrawalState implementation
void CashWithdrawalState::cashWithdrawal(ATM* atm, std::shared_ptr&lt;Card&gt; card, int withdrawAmount) {
    if (atm-&gt;getAtmBalance() &lt; withdrawAmount) {
        std::cout &lt;&lt; "Insufficient fund in the ATM Machine" &lt;&lt; std::endl;
        exit(atm);
    } else if (card-&gt;getBankBalance() &lt; withdrawAmount) {
        std::cout &lt;&lt; "Insufficient fund in your Bank Account" &lt;&lt; std::endl;
        exit(atm);
    } else {
        card-&gt;deductBankBalance(withdrawAmount);
        atm-&gt;deductATMBalance(withdrawAmount);

        // Chain of Responsibility for denomination selection
        auto withdrawProcessor = std::make_shared&lt;TwoThousandWithdrawProcessor&gt;(
            std::make_shared&lt;FiveHundredWithdrawProcessor&gt;(
                std::make_shared&lt;OneHundredWithdrawProcessor&gt;(nullptr)
            )
        );

        withdrawProcessor-&gt;withdraw(atm, withdrawAmount);
        exit(atm);
    }
}

void CashWithdrawalState::exit(ATM* atm) {
    returnCard();
    atm-&gt;setCurrentATMState(std::make_shared&lt;IdleState&gt;());
    std::cout &lt;&lt; "Exit happens" &lt;&lt; std::endl;
}

// CheckBalanceState implementation
void CheckBalanceState::displayBalance(ATM* atm, std::shared_ptr&lt;Card&gt; card) {
    std::cout &lt;&lt; "Your Balance is: " &lt;&lt; card-&gt;getBankBalance() &lt;&lt; std::endl;
    exit(atm);
}

void CheckBalanceState::exit(ATM* atm) {
    returnCard();
    atm-&gt;setCurrentATMState(std::make_shared&lt;IdleState&gt;());
    std::cout &lt;&lt; "Exit happens" &lt;&lt; std::endl;
}

// Cash Withdrawal Processors implementations
void CashWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    if (nextProcessor != nullptr) {
        nextProcessor-&gt;withdraw(atm, remainingAmount);
    }
}

void TwoThousandWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    int required = remainingAmount / 2000;
    int balance = remainingAmount % 2000;

    if (required &lt;= atm-&gt;getNoOfTwoThousandNotes()) {
        atm-&gt;deductTwoThousandNotes(required);
        std::cout &lt;&lt; "Dispensing " &lt;&lt; required &lt;&lt; " notes of 2000" &lt;&lt; std::endl;
    } else if (required &gt; atm-&gt;getNoOfTwoThousandNotes()) {
        int available = atm-&gt;getNoOfTwoThousandNotes();
        atm-&gt;deductTwoThousandNotes(available);
        std::cout &lt;&lt; "Dispensing " &lt;&lt; available &lt;&lt; " notes of 2000" &lt;&lt; std::endl;
        balance = balance + (required - available) * 2000;
    }

    if (balance != 0) {
        CashWithdrawProcessor::withdraw(atm, balance);
    }
}

void FiveHundredWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    int required = remainingAmount / 500;
    int balance = remainingAmount % 500;

    if (required &lt;= atm-&gt;getNoOfFiveHundredNotes()) {
        atm-&gt;deductFiveHundredNotes(required);
        std::cout &lt;&lt; "Dispensing " &lt;&lt; required &lt;&lt; " notes of 500" &lt;&lt; std::endl;
    } else if (required &gt; atm-&gt;getNoOfFiveHundredNotes()) {
        int available = atm-&gt;getNoOfFiveHundredNotes();
        atm-&gt;deductFiveHundredNotes(available);
        std::cout &lt;&lt; "Dispensing " &lt;&lt; available &lt;&lt; " notes of 500" &lt;&lt; std::endl;
        balance = balance + (required - available) * 500;
    }

    if (balance != 0) {
        CashWithdrawProcessor::withdraw(atm, balance);
    }
}

void OneHundredWithdrawProcessor::withdraw(ATM* atm, int remainingAmount) {
    int required = remainingAmount / 100;
    int balance = remainingAmount % 100;

    if (required &lt;= atm-&gt;getNoOfOneHundredNotes()) {
        atm-&gt;deductOneHundredNotes(required);
        std::cout &lt;&lt; "Dispensing " &lt;&lt; required &lt;&lt; " notes of 100" &lt;&lt; std::endl;
    } else if (required &gt; atm-&gt;getNoOfOneHundredNotes()) {
        int available = atm-&gt;getNoOfOneHundredNotes();
        atm-&gt;deductOneHundredNotes(available);
        std::cout &lt;&lt; "Dispensing " &lt;&lt; available &lt;&lt; " notes of 100" &lt;&lt; std::endl;
        balance = balance + (required - available) * 100;
    }

    if (balance != 0) {
        std::cout &lt;&lt; "Cannot dispense remaining: " &lt;&lt; balance &lt;&lt; std::endl;
    }
}
