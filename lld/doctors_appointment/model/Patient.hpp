#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <string>

class Patient {
private:
    std::string patientId;
    std::string name;
    std::string email;
    std::string phone;
    int age;

public:
    Patient(const std::string& patientId, const std::string& name,
            const std::string& email, const std::string& phone, int age = 0)
        : patientId(patientId), name(name), email(email), phone(phone), age(age) {}

    // Getters
    std::string getPatientId() const { return patientId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    int getAge() const { return age; }
};

#endif // PATIENT_HPP
