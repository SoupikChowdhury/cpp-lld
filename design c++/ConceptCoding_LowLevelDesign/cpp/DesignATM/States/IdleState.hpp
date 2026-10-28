#pragma once
#include "ATMState.hpp"

class IdleState : public ATMState {
public:
    void insertCard(ATM* atm, std::shared_ptr<Card> card) override;
};
