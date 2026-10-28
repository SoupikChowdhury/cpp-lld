#ifndef ATM_STATE_HPP
#define ATM_STATE_HPP

#include <string>
#include <memory>
#include "../enums/ATMStatus.hpp"
#include "../model/Card.hpp"

// Forward declaration
class ATMMachine;

class ATMState {
public:
    virtual ~ATMState() = default;
    virtual void insertCard(std::shared_ptr<Card> card) = 0;
    virtual void enterPin(const std::string& pin) = 0;
    virtual void selectOption(const std::string& option) = 0;
    virtual void dispenseCash(int amount) = 0;
    virtual void ejectCard() = 0;
    virtual ATMStatus getStatus() const = 0;
};

#endif // ATM_STATE_HPP
