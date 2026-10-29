#ifndef RATE_LIMIT_CONFIG_HPP
#define RATE_LIMIT_CONFIG_HPP

class RateLimitConfig {
private:
    int maxRequests;
    int windowInSeconds;

public:
    RateLimitConfig(int maxRequests, int windowInSeconds)
        : maxRequests(maxRequests), windowInSeconds(windowInSeconds) {}

    int getMaxRequests() const { return maxRequests; }
    int getWindowInSeconds() const { return windowInSeconds; }
};

#endif // RATE_LIMIT_CONFIG_HPP
