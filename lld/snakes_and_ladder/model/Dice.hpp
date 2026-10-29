#ifndef DICE_HPP
#define DICE_HPP

#include <random>

class Dice {
private:
    int numDice;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    explicit Dice(int numDice = 1) 
        : numDice(numDice), gen(rd()), dis(1, 6) {}

    int roll() {
        int total = 0;
        for (int i = 0; i < numDice; i++) {
            total += dis(gen);
        }
        return total;
    }
};

#endif // DICE_HPP
