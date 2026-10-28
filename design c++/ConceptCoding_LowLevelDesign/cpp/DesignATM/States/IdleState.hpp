#pragma once
#include "ATMState.hpp"

class IdleState : public ATMState {
public:
    void insertCard(ATM* atm, std::shared_ptr&lt;Card&gt; card) override;
};
