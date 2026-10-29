#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "Obstacle.hpp"

// Snake: start > end (moves player down)
class Snake : public Obstacle {
public:
    Snake(int head, int tail) : Obstacle(head, tail) {}

    int getDestination() const override {
        return end;  // Snake takes you down to tail
    }

    ObstacleType getType() const override {
        return ObstacleType::SNAKE;
    }
};

#endif // SNAKE_HPP
