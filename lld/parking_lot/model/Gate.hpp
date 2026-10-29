#ifndef GATE_HPP
#define GATE_HPP

#include <string>
#include "../enums/GateType.hpp"

class Gate {
protected:
    std::string id;

public:
    explicit Gate(const std::string& id) : id(id) {}
    virtual ~Gate() = default;

    std::string getId() const { return id; }
    virtual GateType getType() const = 0;
};

#endif // GATE_HPP
