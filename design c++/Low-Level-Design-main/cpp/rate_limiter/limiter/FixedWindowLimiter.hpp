#ifndef FIXED_WINDOW_LIMITER_HPP
#define FIXED_WINDOW_LIMITER_HPP

#include <unordered_map>
#include <chrono>
#include <mutex>
#include "RateLimiter.hpp"

struct WindowData {
    long long windowStart;
    int requestCount;
    
    WindowData() : windowStart(0), requestCount(0) {}
};

class FixedWindowLimiter : public RateLimiter {
private:
    std::unordered_map<std::string, WindowData> windows;
    std::mutex mtx;

    long long getCurrentWindow() {
        auto now = std::chrono::steady_clock::now();
        auto epoch = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch).count();
        return seconds / config.getWindowInSeconds();
    }

public:
    explicit FixedWindowLimiter(const RateLimitConfig& config) 
        : RateLimiter(config) {}

    bool isAllowed(const std::string& userId) override {
        std::lock_guard<std::mutex> lock(mtx);

        long long currentWindow = getCurrentWindow();

        if (windows.find(userId) == windows.end()) {
            windows[userId] = WindowData();
        }

        WindowData& data = windows[userId];

        // Reset if new window
        if (data.windowStart != currentWindow) {
            data.windowStart = currentWindow;
            data.requestCount = 0;
        }

        if (data.requestCount < config.getMaxRequests()) {
            data.requestCount++;
            return true;
        }

        return false;
    }
};

#endif // FIXED_WINDOW_LIMITER_HPP
