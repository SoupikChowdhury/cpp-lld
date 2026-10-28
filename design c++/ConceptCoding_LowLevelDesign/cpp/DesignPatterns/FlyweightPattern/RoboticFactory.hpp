#pragma once
#include "IRobot.hpp"
#include "HumanoidRobot.hpp"
#include "RoboticDog.hpp"
#include &lt;unordered_map&gt;
#include &lt;memory&gt;
#include &lt;string&gt;

/**
 * Flyweight Factory
 * 
 * Manages pool of flyweight objects. Creates new ones only when needed.
 * Returns cached objects when available.
 */
class RoboticFactory {
private:
    static std::unordered_map&lt;std::string, std::shared_ptr&lt;IRobot&gt;&gt; robotCache;

public:
    static std::shared_ptr&lt;IRobot&gt; createRobot(const std::string&amp; robotType) {
        // Check if robot already exists in cache
        auto it = robotCache.find(robotType);
        if (it != robotCache.end()) {
            std::cout &lt;&lt; "Returning cached " &lt;&lt; robotType &lt;&lt; std::endl;
            return it-&gt;second;
        }

        // Create new robot if not in cache
        std::shared_ptr&lt;IRobot&gt; robot = nullptr;
        
        if (robotType == "HUMANOID") {
            auto sprite = std::make_shared&lt;Sprites&gt;();
            robot = std::make_shared&lt;HumanoidRobot&gt;(robotType, sprite);
        } else if (robotType == "ROBOTICDOG") {
            auto sprite = std::make_shared&lt;Sprites&gt;();
            robot = std::make_shared&lt;RoboticDog&gt;(robotType, sprite);
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
std::unordered_map&lt;std::string, std::shared_ptr&lt;IRobot&gt;&gt; RoboticFactory::robotCache;
