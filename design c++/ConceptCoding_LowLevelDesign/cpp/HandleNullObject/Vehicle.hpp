#pragma once

#include <string>
#include <memory>

namespace NullObjectPattern {

/**
 * @brief Abstract interface for Vehicle
 * Defines the contract that all vehicles must follow
 */
class Vehicle {
public:
    virtual ~Vehicle() = default;

    /**
     * @brief Get the type of vehicle
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Get the number of wheels
     */
    virtual int getNumberOfWheels() const = 0;

    /**
     * @brief Start the vehicle
     */
    virtual void start() const = 0;

    /**
     * @brief Stop the vehicle
     */
    virtual void stop() const = 0;

    /**
     * @brief Get fuel capacity
     */
    virtual double getFuelCapacity() const = 0;

    /**
     * @brief Get seating capacity
     */
    virtual int getSeatingCapacity() const = 0;

    /**
     * @brief Check if vehicle is null
     * This is the key method for Null Object Pattern
     */
    virtual bool isNull() const = 0;

    /**
     * @brief Get vehicle info as string
     */
    virtual std::string toString() const = 0;
};

} // namespace NullObjectPattern
