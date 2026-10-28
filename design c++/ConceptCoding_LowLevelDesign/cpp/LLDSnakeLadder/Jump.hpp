#pragma once

namespace SnakeLadderGame {

enum class JumpType {
    SNAKE,
    LADDER
};

class Jump {
private:
    int startPosition;
    int endPosition;
    JumpType type;

public:
    Jump(int start, int end)
        : startPosition(start), endPosition(end) {
        // Automatically determine type based on positions
        type = (end > start) ? JumpType::LADDER : JumpType::SNAKE;
    }

    int getStartPosition() const { return startPosition; }
    int getEndPosition() const { return endPosition; }
    JumpType getType() const { return type; }

    bool isSnake() const { return type == JumpType::SNAKE; }
    bool isLadder() const { return type == JumpType::LADDER; }

    int getJumpDistance() const {
        return endPosition - startPosition;
    }

    const char* getTypeName() const {
        return type == JumpType::SNAKE ? "SNAKE" : "LADDER";
    }
};

} // namespace SnakeLadderGame
