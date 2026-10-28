#pragma once

#include <string>

namespace Cricbuzz {

/**
 * @brief Base class representing a person
 */
class Person {
protected:
    std::string name_;
    int age_;
    std::string country_;

public:
    Person(const std::string& name, int age, const std::string& country)
        : name_(name), age_(age), country_(country) {}

    virtual ~Person() = default;

    // Getters
    std::string getName() const { return name_; }
    int getAge() const { return age_; }
    std::string getCountry() const { return country_; }

    virtual std::string toString() const {
        return name_ + " (Age: " + std::to_string(age_) + ", " + country_ + ")";
    }
};

} // namespace Cricbuzz
