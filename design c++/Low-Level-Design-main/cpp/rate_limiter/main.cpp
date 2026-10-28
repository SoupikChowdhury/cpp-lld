// Rate Limiter - Main Entry Point
// Demonstrates Factory, Strategy, and Template Method patterns

#include <iostream>
#include <thread>
#include <chrono>
#include "model/User.hpp"
#include "model/RateLimitConfig.hpp"
#include "factory/RateLimiterFactory.hpp"
#include "service/RateLimiterService.hpp"

int main() {
    std::cout << "=== Rate Limiter Demo ===" << std::endl;
    std::cout << std::endl;

    // Create rate limiter service
    RateLimiterService service;

    // Configure different rate limits for different user tiers
    // FREE users: 5 requests per 10 seconds (Token Bucket)
    RateLimitConfig freeConfig(5, 10);
    service.registerLimiter(
        UserTier::FREE,
        RateLimiterFactory::create(RateLimitType::TOKEN_BUCKET, freeConfig)
    );

    // PREMIUM users: 20 requests per 10 seconds (Sliding Window)
    RateLimitConfig premiumConfig(20, 10);
    service.registerLimiter(
        UserTier::PREMIUM,
        RateLimiterFactory::create(RateLimitType::SLIDING_WINDOW_LOG, premiumConfig)
    );

    // Create test users
    User freeUser("user1", UserTier::FREE);
    User premiumUser("user2", UserTier::PREMIUM);

    std::cout << "--- Testing FREE User (5 req/10s - Token Bucket) ---" << std::endl;
    for (int i = 1; i <= 8; i++) {
        bool allowed = service.isAllowed(freeUser);
        std::cout << "Request " << i << ": " 
                  << (allowed ? "ALLOWED" : "RATE LIMITED") << std::endl;
    }

    std::cout << std::endl << "--- Testing PREMIUM User (20 req/10s - Sliding Window) ---" << std::endl;
    for (int i = 1; i <= 25; i++) {
        bool allowed = service.isAllowed(premiumUser);
        std::cout << "Request " << i << ": " 
                  << (allowed ? "ALLOWED" : "RATE LIMITED") << std::endl;
    }

    std::cout << std::endl << "--- Waiting for token refill (3 seconds) ---" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << std::endl << "--- Testing FREE User again after wait ---" << std::endl;
    for (int i = 1; i <= 3; i++) {
        bool allowed = service.isAllowed(freeUser);
        std::cout << "Request " << i << ": " 
                  << (allowed ? "ALLOWED" : "RATE LIMITED") << std::endl;
    }

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
