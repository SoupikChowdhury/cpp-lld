#ifndef TWO_THOUSAND_DISPENSER_HPP
#define TWO_THOUSAND_DISPENSER_HPP

#include <iostream>
#include "CashDispenser.hpp"

class TwoThousandDispenser : public CashDispenser {
private:
    CashDispenser* nextDispenser = nullptr;
    static constexpr int DENOMINATION = 2000;

public:
    void setNext(CashDispenser* next) override {
        nextDispenser = next;
    }

    bool canDispense(ATM* atm, int amount) override {
        int notesNeeded = amount / DENOMINATION;
        int notesAvailable = atm->getTwoThousandNotes();
        int notesToUse = std::min(notesNeeded, notesAvailable);
        int remaining = amount - (notesToUse * DENOMINATION);

        if (remaining == 0) return true;
        if (nextDispenser) return nextDispenser->canDispense(atm, remaining);
        return false;
    }

    void dispense(ATM* atm, int& amount) override {
        int notesNeeded = amount / DENOMINATION;
        int notesAvailable = atm->getTwoThousandNotes();
        int notesToUse = std::min(notesNeeded, notesAvailable);

        if (notesToUse > 0) {
            atm->setTwoThousandNotes(notesAvailable - notesToUse);
            amount -= notesToUse * DENOMINATION;
            std::cout << "Dispensing " << notesToUse << " x Rs.2000 notes" << std::endl;
        }

        if (amount > 0 && nextDispenser) {
            nextDispenser->dispense(atm, amount);
        }
    }
};

#endif // TWO_THOUSAND_DISPENSER_HPP
