#ifndef HIGHEST_RATED_DOCTOR_STRATEGY_HPP
#define HIGHEST_RATED_DOCTOR_STRATEGY_HPP

#include <algorithm>
#include "SlotRankingStrategy.hpp"

class HighestRatedDoctorStrategy : public SlotRankingStrategy {
public:
    std::vector<Appointment*> rankSlots(std::vector<Appointment*>& slots) override {
        std::vector<Appointment*> ranked = slots;
        
        std::sort(ranked.begin(), ranked.end(), 
            [](Appointment* a, Appointment* b) {
                // Sort by rating descending, then by start time
                if (a->getDoctor()->getRating() != b->getDoctor()->getRating()) {
                    return a->getDoctor()->getRating() > b->getDoctor()->getRating();
                }
                return a->getSlot().getStartTime() < b->getSlot().getStartTime();
            });
        
        return ranked;
    }

    std::string getStrategyName() const override {
        return "HighestRatedDoctorStrategy";
    }
};

#endif // HIGHEST_RATED_DOCTOR_STRATEGY_HPP
