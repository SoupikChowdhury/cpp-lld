#ifndef SLOT_RANKING_STRATEGY_HPP
#define SLOT_RANKING_STRATEGY_HPP

#include <vector>
#include "../model/Appointment.hpp"

class SlotRankingStrategy {
protected:
    // Earlier start first; the older slot wins a tie, so results don't vary between runs
    static bool startsFirst(const Appointment* a, const Appointment* b) {
        if (a->getSlot().getStartTime() != b->getSlot().getStartTime()) {
            return a->getSlot().getStartTime() < b->getSlot().getStartTime();
        }
        return a->getAppointmentId() < b->getAppointmentId();
    }

public:
    virtual ~SlotRankingStrategy() = default;
    virtual std::vector<Appointment*> rankSlots(std::vector<Appointment*>& slots) = 0;
    virtual std::string getStrategyName() const = 0;
};

#endif // SLOT_RANKING_STRATEGY_HPP
