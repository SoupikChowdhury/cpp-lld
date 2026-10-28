#pragma once
#include "IRobot.hpp"
#include "HumanoidRobot.hpp"
#include "RoboticDog.hpp"
#include <unordered_map>
#include <memory>
#include <string>

/**
 * Flyweight Factory
 * 
 * Manages pool of flyweight objects. Creates new ones only when needed.
 * Returns cached objects when available.
 */
class RoboticFactory {
private:
    static std::unordered_map<std::string, std::shared_ptr<IRobot>> robotCache;

public:
    static std::shared_ptr<IRobot> createRobot(const std::string& robotType) {
        // Check if robot already exists in cache
        auto it = robotCache.find(robotType);
        if (it != robotCache.end()) {
            std::cout << "Returning cached " << robotType << std::endl;
            return it->second;
        }

        // Create new robot if not in cache
        std::shared_ptr<IRobot> robot = nullptr;
        
        if (robotType == "HUMANOID") {
            auto sprite = std::make_shared<Sprites>();
            robot = std::make_shared<HumanoidRobot>(robotType, sprite);
        } else if (robotType == "ROBOTICDOG") {
            auto sprite = std::make_shared<Sprites>();
            robot = std::make_shared<RoboticDog>(robotType, sprite);
        }

        if (robot) {
            robotCache[robotType] = robot;
        }
        return robot;
    }

    static size_t getCacheSize() {
        return robotCache.size();
    }
};

// Initialize static member
std::unordered_map<std::string, std::shared_ptr<IRobot>> RoboticFactory::robotCache;
