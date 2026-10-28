#pragma once
#include "TransactionType.hpp"
#include "Card.hpp"
#include &lt;memory&gt;
#include &lt;iostream&gt;

// Forward declaration
class ATM;

/**
 * State Pattern - Abstract ATM State
 */
class ATMState {
public:
    virtual ~ATMState() = default;

    virtual void insertCard(ATM* atm, std::shared_ptr&lt;Card&gt; card) {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }

    virtual void authenticatePin(ATM* atm, std::shared_ptr&lt;Card&gt; card, int pin) {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }

    virtual void selectOperation(ATM* atm, std::shared_ptr&lt;Card&gt; card, TransactionType txnType) {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }

    virtual void cashWithdrawal(ATM* atm, std::shared_ptr&lt;Card&gt; card, int withdrawAmount) {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }

    virtual void displayBalance(ATM* atm, std::shared_ptr&lt;Card&gt; card) {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }

    virtual void returnCard() {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }

    virtual void exit(ATM* atm) {
        std::cout &lt;&lt; "OOPS!! Something went wrong" &lt;&lt; std::endl;
    }
};
