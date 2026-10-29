#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "../enums/UserTier.hpp"

class User {
private:
    std::string userId;
    UserTier tier;

public:
    User(const std::string& userId, UserTier tier)
        : userId(userId), tier(tier) {}

    std::string getUserId() const { return userId; }
    UserTier getTier() const { return tier; }
};

#endif // USER_HPP
