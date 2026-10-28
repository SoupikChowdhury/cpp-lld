#ifndef ATM_STATUS_HPP
#define ATM_STATUS_HPP

enum class ATMStatus {
    IDLE,
    CARD_INSERTED,
    AUTHENTICATED,
    DISPENSE_CASH
};

#endif // ATM_STATUS_HPP
