#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "Reservation.hpp"
#include "Payment.hpp"

namespace CarRentalSystem {

class Bill {
private:
    std::string billId;
    std::shared_ptr<Reservation> reservation;
    double baseAmount;
    double taxes;
    double insuranceAmount;
    double lateFeePenalty;
    double damagePenalty;
    double discount;
    double totalAmount;
    bool isPaid;

    static constexpr double TAX_RATE = 0.10;  // 10% tax
    static constexpr double INSURANCE_RATE = 0.05;  // 5% insurance
    static constexpr double LATE_FEE_PER_HOUR = 10.0;

public:
    Bill(const std::string& id, std::shared_ptr<Reservation> reservation)
        : billId(id), reservation(reservation), 
          lateFeePenalty(0), damagePenalty(0), discount(0), isPaid(false) {
        calculateAmounts();
    }

    void calculateAmounts() {
        baseAmount = reservation->calculateBaseAmount();
        taxes = baseAmount * TAX_RATE;
        insuranceAmount = baseAmount * INSURANCE_RATE;
        calculateTotal();
    }

    void calculateTotal() {
        totalAmount = baseAmount + taxes + insuranceAmount + lateFeePenalty + damagePenalty - discount;
    }

    // Getters
    const std::string& getBillId() const { return billId; }
    std::shared_ptr<Reservation> getReservation() const { return reservation; }
    double getBaseAmount() const { return baseAmount; }
    double getTaxes() const { return taxes; }
    double getInsuranceAmount() const { return insuranceAmount; }
    double getLateFeePenalty() const { return lateFeePenalty; }
    double getDamagePenalty() const { return damagePenalty; }
    double getDiscount() const { return discount; }
    double getTotalAmount() const { return totalAmount; }
    bool getIsPaid() const { return isPaid; }

    // Setters for penalties and discounts
    void addLateFee(int hoursLate) {
        lateFeePenalty = hoursLate * LATE_FEE_PER_HOUR;
        calculateTotal();
    }

    void addDamagePenalty(double amount) {
        damagePenalty = amount;
        calculateTotal();
    }

    void applyDiscount(double amount) {
        discount = amount;
        calculateTotal();
    }

    void applyPercentageDiscount(double percentage) {
        discount = baseAmount * (percentage / 100.0);
        calculateTotal();
    }

    void markAsPaid() {
        isPaid = true;
    }

    void displayBill() const {
        std::cout << "\n========================================" << std::endl;
        std::cout << "              RENTAL BILL" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Bill ID: " << billId << std::endl;
        std::cout << "Reservation: " << reservation->getReservationId() << std::endl;
        std::cout << "Customer: " << reservation->getUser()->getName() << std::endl;
        std::cout << "Vehicle: " << reservation->getVehicle()->getVehicleInfo() << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Base Amount:     $" << baseAmount << std::endl;
        std::cout << "Taxes (10%):     $" << taxes << std::endl;
        std::cout << "Insurance (5%):  $" << insuranceAmount << std::endl;
        if (lateFeePenalty > 0) {
            std::cout << "Late Fee:        $" << lateFeePenalty << std::endl;
        }
        if (damagePenalty > 0) {
            std::cout << "Damage Penalty:  $" << damagePenalty << std::endl;
        }
        if (discount > 0) {
            std::cout << "Discount:       -$" << discount << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "TOTAL:           $" << totalAmount << std::endl;
        std::cout << "Status: " << (isPaid ? "PAID" : "UNPAID") << std::endl;
        std::cout << "========================================" << std::endl;
    }
};

} // namespace CarRentalSystem
