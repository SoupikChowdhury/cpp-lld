#pragma once

#include <random>
#include <chrono>

namespace SnakeLadderGame {

class Dice {
private:
    int numberOfDice;
    int minValue;
    int maxValue;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    explicit Dice(int count = 1, int min = 1, int max = 6)
        : numberOfDice(count), minValue(min), maxValue(max) {
        // Seed with current time
        unsigned seed = static_cast<unsigned>(
            std::chrono::system_clock::now().time_since_epoch().count()
        );
        rng.seed(seed);
        dist = std::uniform_int_distribution<int>(minValue, maxValue);
    }

    int rollDice() {
        int total = 0;
        for (int i = 0; i < numberOfDice; ++i) {
            total += dist(rng);
        }
        return total;
    }

    int getNumberOfDice() const { return numberOfDice; }
    int getMinValue() const { return minValue; }
    int getMaxValue() const { return maxValue; }
    int getMaxPossibleValue() const { return numberOfDice * maxValue; }
    int getMinPossibleValue() const { return numberOfDice * minValue; }

    void setNumberOfDice(int count) { numberOfDice = count; }
};

} // namespace SnakeLadderGame
