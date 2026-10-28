#ifndef ATM_HPP
#define ATM_HPP

#include <string>
#include "../enums/ATMStatus.hpp"

class ATM {
private:
    std::string id;
    int twoThousandNotes;
    int fiveHundredNotes;
    int oneHundredNotes;
    ATMStatus status;

public:
    ATM(const std::string& id, int twoThousandNotes, int fiveHundredNotes, int oneHundredNotes)
        : id(id), twoThousandNotes(twoThousandNotes), 
          fiveHundredNotes(fiveHundredNotes), oneHundredNotes(oneHundredNotes),
          status(ATMStatus::IDLE) {}

    std::string getId() const { return id; }
    
    double getCashAvailable() const {
        return twoThousandNotes * 2000.0 + fiveHundredNotes * 500.0 + oneHundredNotes * 100.0;
    }

    int getTwoThousandNotes() const { return twoThousandNotes; }
    int getFiveHundredNotes() const { return fiveHundredNotes; }
    int getOneHundredNotes() const { return oneHundredNotes; }
    
    void setTwoThousandNotes(int count) { twoThousandNotes = count; }
    void setFiveHundredNotes(int count) { fiveHundredNotes = count; }
    void setOneHundredNotes(int count) { oneHundredNotes = count; }

    ATMStatus getStatus() const { return status; }
    void setStatus(ATMStatus newStatus) { status = newStatus; }

    void setCashAvailable(double amount) {
        // This is simplified - in reality we'd recalculate notes
    }
};

#endif // ATM_HPP
