#ifndef RATE_LIMITER_FACTORY_HPP
#define RATE_LIMITER_FACTORY_HPP

#include <memory>
#include "../enums/RateLimitType.hpp"
#include "../model/RateLimitConfig.hpp"
#include "../limiter/RateLimiter.hpp"
#include "../limiter/TokenBucketLimiter.hpp"
#include "../limiter/FixedWindowLimiter.hpp"
#include "../limiter/SlidingWindowLogLimiter.hpp"

class RateLimiterFactory {
public:
    static std::unique_ptr<RateLimiter> create(RateLimitType type, 
                                                const RateLimitConfig& config) {
        switch (type) {
            case RateLimitType::TOKEN_BUCKET:
                return std::make_unique<TokenBucketLimiter>(config);
            case RateLimitType::FIXED_WINDOW:
                return std::make_unique<FixedWindowLimiter>(config);
            case RateLimitType::SLIDING_WINDOW_LOG:
                return std::make_unique<SlidingWindowLogLimiter>(config);
            default:
                return std::make_unique<TokenBucketLimiter>(config);
        }
    }
};

#endif // RATE_LIMITER_FACTORY_HPP
