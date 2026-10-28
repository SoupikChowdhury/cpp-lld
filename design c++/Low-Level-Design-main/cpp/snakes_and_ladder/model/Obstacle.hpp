#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "../enums/ObstacleType.hpp"

class Obstacle {
protected:
    int start;
    int end;

public:
    Obstacle(int start, int end) : start(start), end(end) {}
    virtual ~Obstacle() = default;

    int getStart() const { return start; }
    int getEnd() const { return end; }
    
    virtual int getDestination() const = 0;
    virtual ObstacleType getType() const = 0;
};

#endif // OBSTACLE_HPP
