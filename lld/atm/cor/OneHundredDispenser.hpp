#ifndef ONE_HUNDRED_DISPENSER_HPP
#define ONE_HUNDRED_DISPENSER_HPP

#include <iostream>
#include "CashDispenser.hpp"

class OneHundredDispenser : public CashDispenser {
private:
    CashDispenser* nextDispenser = nullptr;
    static constexpr int DENOMINATION = 100;

public:
    void setNext(CashDispenser* next) override {
        nextDispenser = next;
    }

    bool canDispense(ATM* atm, int amount) override {
        int notesNeeded = amount / DENOMINATION;
        int notesAvailable = atm->getOneHundredNotes();
        int notesToUse = std::min(notesNeeded, notesAvailable);
        int remaining = amount - (notesToUse * DENOMINATION);

        return remaining == 0;
    }

    void dispense(ATM* atm, int& amount) override {
        int notesNeeded = amount / DENOMINATION;
        int notesAvailable = atm->getOneHundredNotes();
        int notesToUse = std::min(notesNeeded, notesAvailable);

        if (notesToUse > 0) {
            atm->setOneHundredNotes(notesAvailable - notesToUse);
            amount -= notesToUse * DENOMINATION;
            std::cout << "Dispensing " << notesToUse << " x Rs.100 notes" << std::endl;
        }
    }
};

#endif // ONE_HUNDRED_DISPENSER_HPP
