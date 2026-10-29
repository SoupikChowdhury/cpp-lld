#ifndef EXIT_GATE_HPP
#define EXIT_GATE_HPP

#include <chrono>
#include <string>
#include "Gate.hpp"
#include "../enums/PaymentMode.hpp"
#include "../service/ParkingLot.hpp"

class ExitGate : public Gate {
public:
    explicit ExitGate(const std::string& id) : Gate(id) {}

    GateType getType() const override {
        return GateType::EXIT;
    }

    void unparkVehicle(const std::string& ticketId, 
                       std::chrono::system_clock::time_point exitTime,
                       PaymentMode paymentMode) {
        ParkingLot::getInstance()->unparkVehicle(ticketId, exitTime, paymentMode);
    }
};

#endif // EXIT_GATE_HPP
