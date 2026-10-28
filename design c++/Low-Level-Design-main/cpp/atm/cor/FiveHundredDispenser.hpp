#ifndef FIVE_HUNDRED_DISPENSER_HPP
#define FIVE_HUNDRED_DISPENSER_HPP

#include <iostream>
#include "CashDispenser.hpp"

class FiveHundredDispenser : public CashDispenser {
private:
    CashDispenser* nextDispenser = nullptr;
    static constexpr int DENOMINATION = 500;

public:
    void setNext(CashDispenser* next) override {
        nextDispenser = next;
    }

    bool canDispense(ATM* atm, int amount) override {
        int notesNeeded = amount / DENOMINATION;
        int notesAvailable = atm->getFiveHundredNotes();
        int notesToUse = std::min(notesNeeded, notesAvailable);
        int remaining = amount - (notesToUse * DENOMINATION);

        if (remaining == 0) return true;
        if (nextDispenser) return nextDispenser->canDispense(atm, remaining);
        return false;
    }

    void dispense(ATM* atm, int& amount) override {
        int notesNeeded = amount / DENOMINATION;
        int notesAvailable = atm->getFiveHundredNotes();
        int notesToUse = std::min(notesNeeded, notesAvailable);

        if (notesToUse > 0) {
            atm->setFiveHundredNotes(notesAvailable - notesToUse);
            amount -= notesToUse * DENOMINATION;
            std::cout << "Dispensing " << notesToUse << " x Rs.500 notes" << std::endl;
        }

        if (amount > 0 && nextDispenser) {
            nextDispenser->dispense(atm, amount);
        }
    }
};

#endif // FIVE_HUNDRED_DISPENSER_HPP
