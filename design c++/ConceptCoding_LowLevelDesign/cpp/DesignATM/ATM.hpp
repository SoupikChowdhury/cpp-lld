#pragma once
#include "States/ATMState.hpp"
#include <memory>
#include <iostream>

/**
 * ATM Machine - Singleton Pattern + State Pattern Context
 */
class ATM {
private:
    static ATM* instance;
    std::shared_ptr<ATMState> currentState;
    int atmBalance = 0;
    int noOfTwoThousandNotes = 0;
    int noOfFiveHundredNotes = 0;
    int noOfOneHundredNotes = 0;

    ATM() {}  // Private constructor for Singleton

public:
    // Delete copy constructor and assignment
    ATM(const ATM&) = delete;
    ATM& operator=(const ATM&) = delete;

    static ATM* getATMObject();

    void setCurrentATMState(std::shared_ptr<ATMState> state) {
        currentState = state;
    }

    std::shared_ptr<ATMState> getCurrentATMState() const {
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
        std::cout << "ATM Balance: " << atmBalance << std::endl;
        std::cout << "2000 Notes: " << noOfTwoThousandNotes << std::endl;
        std::cout << "500 Notes: " << noOfFiveHundredNotes << std::endl;
        std::cout << "100 Notes: " << noOfOneHundredNotes << std::endl;
    }
};
