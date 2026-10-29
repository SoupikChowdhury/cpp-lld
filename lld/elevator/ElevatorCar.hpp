#pragma once

#include <memory>
#include <set>
#include "Direction.hpp"
#include "ElevatorState.hpp"
#include "ElevatorDisplay.hpp"
#include "ElevatorDoor.hpp"
#include "InternalButtons.hpp"

namespace ElevatorSystem {

class ElevatorCar {
private:
    int id;
    int currentFloor;
    Direction direction;
    ElevatorState state;
    std::unique_ptr<ElevatorDisplay> display;
    std::unique_ptr<ElevatorDoor> door;
    std::unique_ptr<InternalButtons> internalButtons;

public:
    ElevatorCar(int id, int minFloor, int maxFloor)
        : id(id),
          currentFloor(0),
          direction(Direction::NONE),
          state(ElevatorState::IDLE),
          display(std::make_unique<ElevatorDisplay>()),
          door(std::make_unique<ElevatorDoor>()),
          internalButtons(std::make_unique<InternalButtons>(minFloor, maxFloor)) {}

    // Getters
    int getId() const { return id; }
    int getCurrentFloor() const { return currentFloor; }
    Direction getDirection() const { return direction; }
    ElevatorState getState() const { return state; }
    ElevatorDisplay* getDisplay() { return display.get(); }
    ElevatorDoor* getDoor() { return door.get(); }
    InternalButtons* getInternalButtons() { return internalButtons.get(); }

    // Setters
    void setCurrentFloor(int floor) { 
        currentFloor = floor;
        display->setDisplay(floor, direction);
    }

    void setDirection(Direction dir) { 
        direction = dir;
        display->setDisplay(currentFloor, dir);
    }

    void setState(ElevatorState s) { state = s; }

    // Operations
    void moveUp() {
        setDirection(Direction::UP);
        setState(ElevatorState::MOVING);
        setCurrentFloor(currentFloor + 1);
        std::cout << "Elevator " << id << " moving UP to floor " << currentFloor << std::endl;
    }

    void moveDown() {
        setDirection(Direction::DOWN);
        setState(ElevatorState::MOVING);
        setCurrentFloor(currentFloor - 1);
        std::cout << "Elevator " << id << " moving DOWN to floor " << currentFloor << std::endl;
    }

    void stop() {
        setState(ElevatorState::STOPPED);
        setDirection(Direction::NONE);
        std::cout << "Elevator " << id << " stopped at floor " << currentFloor << std::endl;
    }

    void openDoor() {
        door->open();
    }

    void closeDoor() {
        door->close();
    }

    void pressInternalButton(int floor) {
        internalButtons->pressButton(floor);
    }

    void showStatus() const {
        std::cout << "Elevator " << id << ": Floor=" << currentFloor 
                  << ", State=" << stateToString(state)
                  << ", Direction=" << directionToString(direction) << std::endl;
    }
};

} // namespace ElevatorSystem
