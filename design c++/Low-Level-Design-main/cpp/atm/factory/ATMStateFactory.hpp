#ifndef ATM_STATE_FACTORY_HPP
#define ATM_STATE_FACTORY_HPP

#include <memory>
#include "../enums/ATMStatus.hpp"
#include "../state/ATMState.hpp"
#include "../state/IdleState.hpp"
#include "../state/CardInsertedState.hpp"
#include "../state/AuthenticatedState.hpp"
#include "../state/DispenseCashState.hpp"

class ATMStateFactory {
public:
    static std::unique_ptr<ATMState> getState(ATMStatus status, ATMMachine* machine) {
        switch (status) {
            case ATMStatus::IDLE:
                return std::make_unique<IdleState>(machine);
            case ATMStatus::CARD_INSERTED:
                return std::make_unique<CardInsertedState>(machine);
            case ATMStatus::AUTHENTICATED:
                return std::make_unique<AuthenticatedState>(machine);
            case ATMStatus::DISPENSE_CASH:
                return std::make_unique<DispenseCashState>(machine);
            default:
                return std::make_unique<IdleState>(machine);
        }
    }
};

#endif // ATM_STATE_FACTORY_HPP
