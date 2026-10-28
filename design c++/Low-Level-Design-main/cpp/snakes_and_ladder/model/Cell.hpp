#ifndef CELL_HPP
#define CELL_HPP

#include "Obstacle.hpp"

class Cell {
private:
    int position;
    Obstacle* obstacle;

public:
    explicit Cell(int position) : position(position), obstacle(nullptr) {}

    int getPosition() const { return position; }
    
    void setObstacle(Obstacle* obs) { obstacle = obs; }
    Obstacle* getObstacle() const { return obstacle; }
    bool hasObstacle() const { return obstacle != nullptr; }
};

#endif // CELL_HPP
