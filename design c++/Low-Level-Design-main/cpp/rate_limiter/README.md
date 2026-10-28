# Rate Limiter - Low Level Design (C++)

## Design Patterns Used
- **Factory Pattern**: RateLimiterFactory creates appropriate rate limiter based on type
- **Strategy Pattern**: Different rate limiting algorithms (Token Bucket, Fixed Window, Sliding Window Log)
- **Template Method**: RateLimiter abstract class with isAllowed() as abstract method

## Class Diagram

```mermaid
classDiagram
    class UserTier {
        <<enumeration>>
        FREE
        PREMIUM
    }
    
    class RateLimitType {
        <<enumeration>>
        TOKEN_BUCKET
        FIXED_WINDOW
        SLIDING_WINDOW_LOG
    }
    
    class RateLimitConfig {
        -int maxRequests
        -int windowInSeconds
        +getMaxRequests() int
        +getWindowInSeconds() int
    }
    
    class User {
        -string userId
        -UserTier tier
        +getUserId() string
        +getTier() UserTier
    }
    
    class RateLimiter {
        <<abstract>>
        #RateLimitConfig config
        +isAllowed(string userId)* bool
    }
    
    class TokenBucketLimiter {
        -map~string, TokenBucket~ buckets
        +isAllowed(string userId) bool
        -refillTokens(TokenBucket bucket)
    }
    
    class FixedWindowLimiter {
        -map~string, WindowData~ windows
        +isAllowed(string userId) bool
    }
    
    class SlidingWindowLogLimiter {
        -map~string, deque~long~~ requestLogs
        +isAllowed(string userId) bool
    }
    
    class RateLimiterFactory {
        +create(RateLimitType type, RateLimitConfig config)$ RateLimiter*
    }
    
    class RateLimiterService {
        -map~UserTier, RateLimiter*~ limiters
        +isAllowed(User user) bool
        +registerLimiter(UserTier tier, RateLimiter* limiter)
    }

    RateLimiter <|-- TokenBucketLimiter
    RateLimiter <|-- FixedWindowLimiter
    RateLimiter <|-- SlidingWindowLogLimiter
    
    RateLimiter --> RateLimitConfig
    RateLimiterService --> RateLimiter
    User --> UserTier
    
    RateLimiterFactory ..> RateLimiter : creates
```

## Token Bucket Algorithm

```mermaid
flowchart TD
    A[Request Arrives] --> B{Tokens Available?}
    B -->|Yes| C[Consume Token]
    C --> D[Allow Request]
    B -->|No| E[Reject Request]
    
    F[Time Passes] --> G[Refill Tokens]
    G --> H{At Max Capacity?}
    H -->|No| G
    H -->|Yes| I[Stop Refilling]
```

## Fixed Window Algorithm

```mermaid
flowchart TD
    A[Request Arrives] --> B[Get Current Window]
    B --> C{Same Window?}
    C -->|No| D[Reset Counter]
    D --> E[Increment Counter]
    C -->|Yes| E
    E --> F{Counter <= Max?}
    F -->|Yes| G[Allow Request]
    F -->|No| H[Reject Request]
```

## Sliding Window Log Algorithm

```mermaid
flowchart TD
    A[Request Arrives] --> B[Get Current Time]
    B --> C[Remove Old Entries]
    C --> D{Log Size < Max?}
    D -->|Yes| E[Add Timestamp to Log]
    E --> F[Allow Request]
    D -->|No| G[Reject Request]
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant Client
    participant Service as RateLimiterService
    participant Limiter as TokenBucketLimiter
    participant Bucket as TokenBucket
    
    Client->>Service: isAllowed(user)
    Service->>Service: getLimiter(user.tier)
    Service->>Limiter: isAllowed(userId)
    Limiter->>Bucket: getOrCreate(userId)
    Limiter->>Bucket: refillTokens()
    Limiter->>Bucket: tryConsume()
    alt Tokens Available
        Bucket-->>Limiter: true
        Limiter-->>Service: true
        Service-->>Client: Request Allowed
    else No Tokens
        Bucket-->>Limiter: false
        Limiter-->>Service: false
        Service-->>Client: Rate Limited
    end
```

## Files Structure
```
rate_limiter/
├── enums/
│   ├── UserTier.hpp
│   └── RateLimitType.hpp
├── model/
│   ├── User.hpp
│   └── RateLimitConfig.hpp
├── limiter/
│   ├── RateLimiter.hpp
│   ├── TokenBucketLimiter.hpp
│   ├── FixedWindowLimiter.hpp
│   └── SlidingWindowLogLimiter.hpp
├── factory/
│   └── RateLimiterFactory.hpp
├── service/
│   └── RateLimiterService.hpp
└── main.cpp
```
