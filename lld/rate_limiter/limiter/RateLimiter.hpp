#ifndef RATE_LIMITER_HPP
#define RATE_LIMITER_HPP

#include <string>
#include "../model/RateLimitConfig.hpp"

class RateLimiter {
protected:
    RateLimitConfig config;

public:
    explicit RateLimiter(const RateLimitConfig& config) : config(config) {}
    virtual ~RateLimiter() = default;

    virtual bool isAllowed(const std::string& userId) = 0;
};

#endif // RATE_LIMITER_HPP
