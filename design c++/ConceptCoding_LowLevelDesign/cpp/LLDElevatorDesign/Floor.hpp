#pragma once

#include <memory>
#include "ExternalButtons.hpp"

namespace ElevatorSystem {

class Floor {
private:
    int floorNumber;
    std::unique_ptr<ExternalButtons> externalButtons;

public:
    explicit Floor(int number)
        : floorNumber(number),
          externalButtons(std::make_unique<ExternalButtons>(number)) {}

    int getFloorNumber() const { return floorNumber; }
    
    ExternalButtons* getExternalButtons() { return externalButtons.get(); }

    void pressUpButton() {
        externalButtons->pressUp();
    }

    void pressDownButton() {
        externalButtons->pressDown();
    }

    void resetButtons() {
        externalButtons->resetUp();
        externalButtons->resetDown();
    }
};

} // namespace ElevatorSystem
