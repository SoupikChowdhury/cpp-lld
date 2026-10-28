#ifndef GROUP_REPOSITORY_HPP
#define GROUP_REPOSITORY_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "../model/Group.hpp"

class GroupRepository {
private:
    std::unordered_map<std::string, std::unique_ptr<Group>> groups;

public:
    void save(std::unique_ptr<Group> group) {
        groups[group->getId()] = std::move(group);
    }

    Group* findById(const std::string& id) {
        auto it = groups.find(id);
        return (it != groups.end()) ? it->second.get() : nullptr;
    }
};

#endif // GROUP_REPOSITORY_HPP
