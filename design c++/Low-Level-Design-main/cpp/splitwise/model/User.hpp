#ifndef SPLITWISE_USER_HPP
#define SPLITWISE_USER_HPP

#include <string>

class User {
private:
    std::string id;
    std::string name;

public:
    User(const std::string& id, const std::string& name) 
        : id(id), name(name) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
};

#endif // SPLITWISE_USER_HPP
