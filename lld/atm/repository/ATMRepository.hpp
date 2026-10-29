#ifndef ATM_REPOSITORY_HPP
#define ATM_REPOSITORY_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include "../model/ATM.hpp"
#include "../enums/ATMStatus.hpp"

class ATMRepository {
private:
    std::unordered_map<std::string, std::shared_ptr<ATM>> atms;

public:
    void save(std::shared_ptr<ATM> atm) {
        atms[atm->getId()] = atm;
    }

    std::shared_ptr<ATM> getById(const std::string& id) {
        auto it = atms.find(id);
        if (it != atms.end()) {
            return it->second;
        }
        return nullptr;
    }

    void updateATMStatusById(const std::string& id, ATMStatus newStatus) {
        auto it = atms.find(id);
        if (it != atms.end()) {
            it->second->setStatus(newStatus);
        }
    }
};

#endif // ATM_REPOSITORY_HPP
