#ifndef OBSTACLE_FACTORY_HPP
#define OBSTACLE_FACTORY_HPP

#include <memory>
#include "../enums/ObstacleType.hpp"
#include "../model/Obstacle.hpp"
#include "../model/Snake.hpp"
#include "../model/Ladder.hpp"

class ObstacleFactory {
public:
    static Obstacle* create(ObstacleType type, int start, int end) {
        switch (type) {
            case ObstacleType::SNAKE:
                return new Snake(start, end);
            case ObstacleType::LADDER:
                return new Ladder(start, end);
            default:
                return nullptr;
        }
    }
};

#endif // OBSTACLE_FACTORY_HPP
