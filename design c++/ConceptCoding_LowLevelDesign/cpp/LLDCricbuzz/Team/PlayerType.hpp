#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Enum for player types in cricket
 */
enum class PlayerType {
    BATSMAN,
    BOWLER,
    ALL_ROUNDER,
    WICKET_KEEPER
};

/**
 * @brief Utility to convert PlayerType to string
 */
inline std::string playerTypeToString(PlayerType type) {
    switch (type) {
        case PlayerType::BATSMAN: return "Batsman";
        case PlayerType::BOWLER: return "Bowler";
        case PlayerType::ALL_ROUNDER: return "All-Rounder";
        case PlayerType::WICKET_KEEPER: return "Wicket Keeper";
        default: return "Unknown";
    }
}

} // namespace Cricbuzz
