#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Group.hpp"
#include "../User/User.hpp"

/**
 * @brief Controller for managing groups in Splitwise
 * 
 * Handles creation and retrieval of groups.
 */
class GroupController {
private:
    std::vector<std::shared_ptr<Group>> groupList;

public:
    GroupController() = default;

    /**
     * @brief Create a new group
     * @param groupId Unique identifier for the group
     * @param groupName Name of the group
     * @param createdByUser User who created the group (automatically added as member)
     */
    void createNewGroup(
        const std::string& groupId,
        const std::string& groupName,
        std::shared_ptr<User> createdByUser) {
        
        auto group = std::make_shared<Group>();
        group->setGroupId(groupId);
        group->setGroupName(groupName);
        
        // Add the creator as the first member
        group->addMember(createdByUser);
        
        groupList.push_back(group);
    }

    /**
     * @brief Get a group by its ID
     * @param groupId The unique identifier of the group
     * @return Shared pointer to the group, or nullptr if not found
     */
    std::shared_ptr<Group> getGroup(const std::string& groupId) {
        for (auto& group : groupList) {
            if (group->getGroupId() == groupId) {
                return group;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get all groups
     * @return Reference to the vector of all groups
     */
    const std::vector<std::shared_ptr<Group>>& getAllGroups() const {
        return groupList;
    }
};
