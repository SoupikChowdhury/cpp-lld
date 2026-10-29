#pragma once

#include <string>

namespace SnakeLadderGame {

class Player {
private:
    std::string id;
    std::string name;
    int currentPosition;

public:
    Player(const std::string& id, const std::string& name)
        : id(id), name(name), currentPosition(0) {}

    // Getters
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    int getCurrentPosition() const { return currentPosition; }

    // Setters
    void setCurrentPosition(int position) { currentPosition = position; }

    // Move player
    void moveTo(int position) {
        currentPosition = position;
    }

    void reset() {
        currentPosition = 0;
    }
};

} // namespace SnakeLadderGame
