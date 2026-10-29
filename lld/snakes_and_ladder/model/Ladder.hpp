#ifndef LADDER_HPP
#define LADDER_HPP

#include "Obstacle.hpp"

// Ladder: start < end (moves player up)
class Ladder : public Obstacle {
public:
    Ladder(int bottom, int top) : Obstacle(bottom, top) {}

    int getDestination() const override {
        return end;  // Ladder takes you up to top
    }

    ObstacleType getType() const override {
        return ObstacleType::LADDER;
    }
};

#endif // LADDER_HPP
