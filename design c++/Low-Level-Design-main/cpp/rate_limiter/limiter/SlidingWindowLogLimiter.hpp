#ifndef SLIDING_WINDOW_LOG_LIMITER_HPP
#define SLIDING_WINDOW_LOG_LIMITER_HPP

#include <unordered_map>
#include <deque>
#include <chrono>
#include <mutex>
#include "RateLimiter.hpp"

class SlidingWindowLogLimiter : public RateLimiter {
private:
    std::unordered_map<std::string, std::deque<long long>> requestLogs;
    std::mutex mtx;

    long long getCurrentTimeMillis() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();
    }

    void removeOldEntries(std::deque<long long>& log, long long windowStart) {
        while (!log.empty() && log.front() < windowStart) {
            log.pop_front();
        }
    }

public:
    explicit SlidingWindowLogLimiter(const RateLimitConfig& config) 
        : RateLimiter(config) {}

    bool isAllowed(const std::string& userId) override {
        std::lock_guard<std::mutex> lock(mtx);

        long long now = getCurrentTimeMillis();
        long long windowStart = now - (config.getWindowInSeconds() * 1000);

        if (requestLogs.find(userId) == requestLogs.end()) {
            requestLogs[userId] = std::deque<long long>();
        }

        std::deque<long long>& log = requestLogs[userId];
        
        // Remove old entries outside the window
        removeOldEntries(log, windowStart);

        if (static_cast<int>(log.size()) < config.getMaxRequests()) {
            log.push_back(now);
            return true;
        }

        return false;
    }
};

#endif // SLIDING_WINDOW_LOG_LIMITER_HPP
