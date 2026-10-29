#ifndef RATE_LIMITER_SERVICE_HPP
#define RATE_LIMITER_SERVICE_HPP

#include <unordered_map>
#include <memory>
#include "../enums/UserTier.hpp"
#include "../model/User.hpp"
#include "../limiter/RateLimiter.hpp"

class RateLimiterService {
private:
    std::unordered_map<UserTier, std::unique_ptr<RateLimiter>> limiters;

public:
    void registerLimiter(UserTier tier, std::unique_ptr<RateLimiter> limiter) {
        limiters[tier] = std::move(limiter);
    }

    bool isAllowed(const User& user) {
        auto it = limiters.find(user.getTier());
        if (it != limiters.end()) {
            return it->second->isAllowed(user.getUserId());
        }
        return false;  // No limiter configured, deny by default
    }
};

#endif // RATE_LIMITER_SERVICE_HPP
