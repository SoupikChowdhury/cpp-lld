#ifndef ENTRY_GATE_HPP
#define ENTRY_GATE_HPP

#include <chrono>
#include <memory>
#include "Gate.hpp"
#include "Vehicle.hpp"
#include "Ticket.hpp"
#include "../service/ParkingLot.hpp"

class EntryGate : public Gate {
public:
    explicit EntryGate(const std::string& id) : Gate(id) {}

    GateType getType() const override {
        return GateType::ENTRY;
    }

    Ticket* parkVehicle(std::shared_ptr<Vehicle> vehicle, 
                        std::chrono::system_clock::time_point entryTime) {
        return ParkingLot::getInstance()->parkVehicle(vehicle, entryTime);
    }
};

#endif // ENTRY_GATE_HPP
