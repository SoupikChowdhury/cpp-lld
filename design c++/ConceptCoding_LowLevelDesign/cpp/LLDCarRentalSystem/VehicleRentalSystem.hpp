#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <ctime>
#include <iostream>
#include "Store.hpp"
#include "User.hpp"
#include "Bill.hpp"
#include "Payment.hpp"
#include "Product/Car.hpp"
#include "Product/Bike.hpp"

namespace CarRentalSystem {

// Singleton Pattern for the main rental system
class VehicleRentalSystem {
private:
    std::string systemName;
    std::unordered_map<std::string, std::shared_ptr<Store>> stores;
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::unordered_map<std::string, std::shared_ptr<Bill>> bills;
    std::unordered_map<std::string, std::shared_ptr<Payment>> payments;
    int reservationCounter;
    int billCounter;
    int paymentCounter;

    VehicleRentalSystem() 
        : systemName("Vehicle Rental System"), 
          reservationCounter(0), billCounter(0), paymentCounter(0) {}

public:
    // Delete copy constructor and assignment operator
    VehicleRentalSystem(const VehicleRentalSystem&) = delete;
    VehicleRentalSystem& operator=(const VehicleRentalSystem&) = delete;

    static VehicleRentalSystem& getInstance() {
        static VehicleRentalSystem instance;
        return instance;
    }

    // Store management
    void addStore(std::shared_ptr<Store> store) {
        stores[store->getStoreId()] = store;
        std::cout << "Store added: " << store->getName() << std::endl;
    }

    std::shared_ptr<Store> getStore(const std::string& storeId) {
        auto it = stores.find(storeId);
        return (it != stores.end()) ? it->second : nullptr;
    }

    std::vector<std::shared_ptr<Store>> getAllStores() {
        std::vector<std::shared_ptr<Store>> result;
        for (const auto& [id, store] : stores) {
            result.push_back(store);
        }
        return result;
    }

    // User management
    void registerUser(std::shared_ptr<User> user) {
        users[user->getUserId()] = user;
        std::cout << "User registered: " << user->getName() << std::endl;
    }

    std::shared_ptr<User> getUser(const std::string& userId) {
        auto it = users.find(userId);
        return (it != users.end()) ? it->second : nullptr;
    }

    // Search vehicles across all stores
    std::vector<std::pair<std::shared_ptr<Store>, std::shared_ptr<Vehicle>>> 
    searchAvailableVehicles(VehicleType type) {
        std::vector<std::pair<std::shared_ptr<Store>, std::shared_ptr<Vehicle>>> results;
        for (const auto& [id, store] : stores) {
            auto vehicles = store->getAvailableVehiclesByType(type);
            for (const auto& vehicle : vehicles) {
                results.push_back({store, vehicle});
            }
        }
        return results;
    }

    // Make reservation
    std::shared_ptr<Reservation> makeReservation(
        const std::string& storeId,
        const std::string& userId,
        const std::string& vehicleId,
        std::shared_ptr<Location> dropoffLocation,
        time_t pickupTime,
        int duration,
        ReservationType type
    ) {
        auto store = getStore(storeId);
        auto user = getUser(userId);
        
        if (!store || !user) {
            std::cout << "Store or User not found!" << std::endl;
            return nullptr;
        }

        auto vehicle = store->getInventory()->getVehicle(vehicleId);
        if (!vehicle || vehicle->getStatus() != VehicleStatus::AVAILABLE) {
            std::cout << "Vehicle not available!" << std::endl;
            return nullptr;
        }

        std::string reservationId = "RES" + std::to_string(++reservationCounter);
        auto reservation = store->createReservation(
            reservationId, user, vehicle, dropoffLocation, pickupTime, duration, type
        );

        reservation->confirm();
        std::cout << "Reservation created: " << reservationId << std::endl;
        return reservation;
    }

    // Generate bill
    std::shared_ptr<Bill> generateBill(std::shared_ptr<Reservation> reservation) {
        std::string billId = "BILL" + std::to_string(++billCounter);
        auto bill = std::make_shared<Bill>(billId, reservation);
        bills[billId] = bill;
        return bill;
    }

    // Process payment
    std::shared_ptr<Payment> processPayment(
        std::shared_ptr<Bill> bill,
        PaymentMode mode
    ) {
        std::string paymentId = "PAY" + std::to_string(++paymentCounter);
        auto payment = std::make_shared<Payment>(
            paymentId, 
            bill->getReservation()->getReservationId(),
            bill->getTotalAmount(),
            mode
        );

        if (payment->processPayment()) {
            bill->markAsPaid();
            payments[paymentId] = payment;
            return payment;
        }
        return nullptr;
    }

    // Complete rental
    void completeRental(const std::string& storeId, const std::string& reservationId,
                        int kmDriven = 0, int hoursLate = 0, double damageCost = 0) {
        auto store = getStore(storeId);
        if (!store) return;

        auto reservation = store->getReservation(reservationId);
        if (!reservation) return;

        // Update vehicle
        auto vehicle = reservation->getVehicle();
        vehicle->addKmDriven(kmDriven);

        // Generate bill with any penalties
        auto bill = generateBill(reservation);
        if (hoursLate > 0) {
            bill->addLateFee(hoursLate);
        }
        if (damageCost > 0) {
            bill->addDamagePenalty(damageCost);
        }

        bill->displayBill();

        // Complete reservation
        reservation->complete();
        std::cout << "Rental completed for reservation: " << reservationId << std::endl;
    }

    void displaySystemStatus() const {
        std::cout << "\n========================================" << std::endl;
        std::cout << "    " << systemName << " Status" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Total Stores: " << stores.size() << std::endl;
        std::cout << "Registered Users: " << users.size() << std::endl;
        std::cout << "Total Reservations: " << reservationCounter << std::endl;
        std::cout << "Total Bills: " << bills.size() << std::endl;
        std::cout << "Total Payments: " << payments.size() << std::endl;
        std::cout << "========================================" << std::endl;
    }
};

} // namespace CarRentalSystem
