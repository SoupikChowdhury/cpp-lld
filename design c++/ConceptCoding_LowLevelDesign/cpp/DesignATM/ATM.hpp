#pragma once
#include "States/ATMState.hpp"
#include &lt;memory&gt;
#include &lt;iostream&gt;

/**
 * ATM Machine - Singleton Pattern + State Pattern Context
 */
class ATM {
private:
    static ATM* instance;
    std::shared_ptr&lt;ATMState&gt; currentState;
    int atmBalance = 0;
    int noOfTwoThousandNotes = 0;
    int noOfFiveHundredNotes = 0;
    int noOfOneHundredNotes = 0;

    ATM() {}  // Private constructor for Singleton

public:
    // Delete copy constructor and assignment
    ATM(const ATM&amp;) = delete;
    ATM&amp; operator=(const ATM&amp;) = delete;

    static ATM* getATMObject();

    void setCurrentATMState(std::shared_ptr&lt;ATMState&gt; state) {
        currentState = state;
    }

    std::shared_ptr&lt;ATMState&gt; getCurrentATMState() const {
        return currentState;
    }

    int getAtmBalance() const { return atmBalance; }

    void setAtmBalance(int balance, int twoK, int fiveH, int oneH) {
        atmBalance = balance;
        noOfTwoThousandNotes = twoK;
        noOfFiveHundredNotes = fiveH;
        noOfOneHundredNotes = oneH;
    }

    int getNoOfTwoThousandNotes() const { return noOfTwoThousandNotes; }
    int getNoOfFiveHundredNotes() const { return noOfFiveHundredNotes; }
    int getNoOfOneHundredNotes() const { return noOfOneHundredNotes; }

    void deductATMBalance(int amount) { atmBalance -= amount; }
    void deductTwoThousandNotes(int n) { noOfTwoThousandNotes -= n; }
    void deductFiveHundredNotes(int n) { noOfFiveHundredNotes -= n; }
    void deductOneHundredNotes(int n) { noOfOneHundredNotes -= n; }

    void printCurrentATMStatus() const {
        std::cout &lt;&lt; "ATM Balance: " &lt;&lt; atmBalance &lt;&lt; std::endl;
        std::cout &lt;&lt; "2000 Notes: " &lt;&lt; noOfTwoThousandNotes &lt;&lt; std::endl;
        std::cout &lt;&lt; "500 Notes: " &lt;&lt; noOfFiveHundredNotes &lt;&lt; std::endl;
        std::cout &lt;&lt; "100 Notes: " &lt;&lt; noOfOneHundredNotes &lt;&lt; std::endl;
    }
};
