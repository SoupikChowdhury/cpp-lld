#ifndef CASH_DISPENSER_HPP
#define CASH_DISPENSER_HPP

#include "../model/ATM.hpp"

class CashDispenser {
public:
    virtual ~CashDispenser() = default;
    virtual void setNext(CashDispenser* next) = 0;
    virtual bool canDispense(ATM* atm, int amount) = 0;
    virtual void dispense(ATM* atm, int& amount) = 0;
};

#endif // CASH_DISPENSER_HPP
