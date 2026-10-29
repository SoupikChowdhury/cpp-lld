#ifndef TOKEN_BUCKET_LIMITER_HPP
#define TOKEN_BUCKET_LIMITER_HPP

#include <unordered_map>
#include <chrono>
#include <mutex>
#include "RateLimiter.hpp"

struct TokenBucket {
    double tokens;
    std::chrono::steady_clock::time_point lastRefillTime;
    
    TokenBucket() : tokens(0), lastRefillTime(std::chrono::steady_clock::now()) {}
};

class TokenBucketLimiter : public RateLimiter {
private:
    std::unordered_map<std::string, TokenBucket> buckets;
    std::mutex mtx;

    void refillTokens(TokenBucket& bucket) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - bucket.lastRefillTime
        ).count();

        if (elapsed > 0) {
            double tokensToAdd = (elapsed * config.getMaxRequests()) / 
                                 static_cast<double>(config.getWindowInSeconds());
            bucket.tokens = std::min(
                static_cast<double>(config.getMaxRequests()),
                bucket.tokens + tokensToAdd
            );
            bucket.lastRefillTime = now;
        }
    }

public:
    explicit TokenBucketLimiter(const RateLimitConfig& config) 
        : RateLimiter(config) {}

    bool isAllowed(const std::string& userId) override {
        std::lock_guard<std::mutex> lock(mtx);

        // Create bucket if not exists
        if (buckets.find(userId) == buckets.end()) {
            buckets[userId] = TokenBucket();
            buckets[userId].tokens = config.getMaxRequests();
        }

        TokenBucket& bucket = buckets[userId];
        refillTokens(bucket);

        if (bucket.tokens >= 1.0) {
            bucket.tokens -= 1.0;
            return true;
        }

        return false;
    }
};

#endif // TOKEN_BUCKET_LIMITER_HPP
