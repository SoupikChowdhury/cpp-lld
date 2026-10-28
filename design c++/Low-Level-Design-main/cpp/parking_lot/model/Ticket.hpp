#ifndef TICKET_HPP
#define TICKET_HPP

#include <string>
#include <memory>
#include <chrono>
#include "Vehicle.hpp"
#include "ParkingSpot.hpp"

class Ticket {
private:
    std::string id;
    std::shared_ptr<Vehicle> vehicle;
    std::shared_ptr<ParkingSpot> spot;
    std::chrono::system_clock::time_point entryTime;
    std::chrono::system_clock::time_point exitTime;
    double fee = 0.0;

public:
    // Builder pattern
    class Builder {
    private:
        std::string id;
        std::shared_ptr<Vehicle> vehicle;
        std::shared_ptr<ParkingSpot> spot;
        std::chrono::system_clock::time_point entryTime;

    public:
        Builder& setId(const std::string& id) {
            this->id = id;
            return *this;
        }

        Builder& setVehicle(std::shared_ptr<Vehicle> vehicle) {
            this->vehicle = vehicle;
            return *this;
        }

        Builder& setSpot(std::shared_ptr<ParkingSpot> spot) {
            this->spot = spot;
            return *this;
        }

        Builder& setEntryTime(std::chrono::system_clock::time_point time) {
            this->entryTime = time;
            return *this;
        }

        std::unique_ptr<Ticket> build() {
            auto ticket = std::make_unique<Ticket>();
            ticket->id = id;
            ticket->vehicle = vehicle;
            ticket->spot = spot;
            ticket->entryTime = entryTime;
            return ticket;
        }
    };

    Ticket() = default;

    std::string getId() const { return id; }
    std::shared_ptr<Vehicle> getVehicle() const { return vehicle; }
    std::shared_ptr<ParkingSpot> getSpot() const { return spot; }
    std::chrono::system_clock::time_point getEntryTime() const { return entryTime; }
    std::chrono::system_clock::time_point getExitTime() const { return exitTime; }
    double getFee() const { return fee; }

    void setExitTime(std::chrono::system_clock::time_point time) { exitTime = time; }
    void setFee(double amount) { fee = amount; }
};

#endif // TICKET_HPP
