#pragma once
#include "IRobot.hpp"
#include "HumanoidRobot.hpp"
#include "RoboticDog.hpp"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

/**
 * Flyweight Factory
 *
 * Manages pool of flyweight objects. Creates new ones only when needed.
 * Returns cached objects when available.
 */
class RoboticFactory {
private:
    // inline: a plain definition in a header breaks the link when two source files include it
    inline static std::unordered_map<std::string, std::shared_ptr<IRobot>> robotCache;
    inline static std::mutex cacheMutex;

public:
    static std::shared_ptr<IRobot> createRobot(const std::string& robotType) {
        std::lock_guard<std::mutex> lock(cacheMutex);

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
        } else {
            throw std::invalid_argument("Unknown robot type: " + robotType);
        }

        robotCache[robotType] = robot;
        return robot;
    }

    static size_t getCacheSize() {
        std::lock_guard<std::mutex> lock(cacheMutex);
        return robotCache.size();
    }
};
