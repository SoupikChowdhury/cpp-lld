#pragma once

#include <memory>
#include "Jump.hpp"

namespace SnakeLadderGame {

class Cell {
private:
    int position;
    std::unique_ptr<Jump> jump;

public:
    explicit Cell(int pos) : position(pos), jump(nullptr) {}

    int getPosition() const { return position; }

    bool hasJump() const { return jump != nullptr; }

    void setJump(std::unique_ptr<Jump> j) {
        jump = std::move(j);
    }

    void removeJump() {
        jump.reset();
    }

    Jump* getJump() const { return jump.get(); }

    // Returns the final position after applying jump (if any)
    int getFinalPosition() const {
        if (jump) {
            return jump->getEndPosition();
        }
        return position;
    }

    bool hasSnake() const {
        return jump && jump->isSnake();
    }

    bool hasLadder() const {
        return jump && jump->isLadder();
    }
};

} // namespace SnakeLadderGame
